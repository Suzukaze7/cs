/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

 /*********************************************************
  * NOTE TO STUDENTS: Before you do anything else, please
  * provide your team information in the following struct.
  ********************************************************/
team_t team = {
    /* Team name */
    "suzukaze",
    /* First member's full name */
    "Suzukaze",
    /* First member's email address */
    "1027743497@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/**
 * 堆的整体结构为一个分离适配的分离空闲链表，每个大小类为 2^(k-1)+1 ~ 2^k，
 * 每个块由头部脚部与载荷和填充部分组成，空闲块将载荷替换为紧邻着头部的后继块指针，
 * 初始时由长为 CLASSNUM 的指针数组作为每个大小类头指针，
 * 数组随后是两个只有头部(尾部)的长度为0标记为使用的块，方便处理边界条件，
 * 而有效块处在这两个块之间
*/

/* 字大小与双字大小 */
#define WSIZE 4
#define DSIZE 8
/* 最小块大小 */
#define MINCHUNKSIZE 16
/* 大小类数量 */
#define CLASSNUM 20
#define FREE 0
#define USE 1
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define errp (void*)-1

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 虽然c的指针可以随便转换，但还是按照cpp的规则来，参数和返回值都是void* */
/* useful function */
static unsigned get(void *p);
static void set(void *p, unsigned val);
static unsigned get_size(void *p);
static unsigned get_alloc(void *p);
static void *get_head(void *bp);
static void *get_tail(void *bp);
static void *last_block(void *bp);
static void *next_block(void *bp);
static unsigned eval_size(unsigned size);
static void *extern_heap(unsigned size);
static void insert_block(void *bp);
static void *find_fit(unsigned size);
static void place(void *bp, unsigned newsize);
static void checker();

static void *heap;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    if ((heap = mem_sbrk((CLASSNUM + 2) * WSIZE)) == errp)
        return -1;

    void **head = heap;
    for (int i = 0; i < CLASSNUM; i++, head++)
        *head = NULL;
    set(head, 0 | USE);
    set(head + 1, 0 | USE);

    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    unsigned newsize = eval_size(size);

    void *bp = find_fit(newsize);
    if (bp == NULL)
        bp = extern_heap(newsize);
    if (bp == NULL)
        return NULL;

    place(bp, newsize);

    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    unsigned size = get_size(get_head(ptr));

    if (get_alloc(ptr - DSIZE) == FREE)
    {
        ptr = last_block(ptr);
        size += get_size(get_head(ptr));
        set(get_head(ptr), size | FREE);
        set(get_tail(ptr), size | FREE);
    }
    if (get_alloc(ptr + size - WSIZE) == FREE)
    {
        size += get_size(ptr + size - WSIZE);
        set(get_head(ptr), size | FREE);
        set(get_tail(ptr), size | FREE);
    }

    insert_block(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldbp = ptr;
    void *newbp;

    size = eval_size(size);
    newbp = mm_malloc(size);
    if (newbp == NULL)
        return NULL;
    unsigned copy_size = get_size(get_head(oldbp));
    if (copy_size > size)
        copy_size = size;

    memcpy(newbp, oldbp, copy_size - DSIZE);
    mm_free(oldbp);

    return newbp;
}

static void *extern_heap(unsigned size)
{
    void *bp = mem_sbrk(size);
    if (bp == errp)
        return NULL;

    set(get_head(bp), size | FREE);
    set(get_tail(bp), size | FREE);
    set(get_tail(bp) + WSIZE, 0 | USE);

    return bp;
}

static void insert_block(void *bp)
{
    void **head = heap;
    for (unsigned i = 1, size = get_size(get_head(bp)); ; i <<= 1, head++)
        if (size <= i)
        {
            void *t = *head;
            *head = bp;
            *(void **)bp = t;

            break;
        }
}

static void *find_fit(unsigned size)
{
    void **head = heap;
    for (unsigned i = 1, j = 0; j < CLASSNUM; j++, i <<= 1, head++)
        if (size <= i)
        {
            for (void **bp = *head, **last = head; bp; last = bp, bp = *bp)
                if (get_size(get_head(bp)) >= size)
                {
                    *last = *bp;
                    return bp;
                }
        }
    return NULL;
}

static void place(void *bp, unsigned newsize)
{
    unsigned size = get_size(get_head(bp));
    if (size - newsize >= MINCHUNKSIZE)
    {
        set(get_head(bp), newsize | USE);
        set(get_tail(bp), newsize | USE);
        bp = next_block(bp);
        set(get_head(bp), size - newsize);
        set(get_tail(bp), size - newsize);
        insert_block(bp);
    }
    else
    {
        set(get_head(bp), size | USE);
        set(get_tail(bp), size | USE);
    }
}

static unsigned get(void *p) { return *(unsigned *)p; }

static void set(void *p, unsigned val) { *(unsigned *)p = val; }

static unsigned get_size(void *p) { return get(p) & -2; }

static unsigned get_alloc(void *p) { return get(p) & 1; }

static void *get_head(void *bp) { return bp - WSIZE; }

static void *get_tail(void *bp) { return bp + get_size(get_head(bp)) - DSIZE; }

static void *last_block(void *bp) { return bp - get_size(bp - DSIZE); }

static void *next_block(void *bp) { return get_tail(bp) + DSIZE; }

static unsigned eval_size(unsigned size)
{
    size = ALIGN(size + DSIZE);
    return size > MINCHUNKSIZE ? size : MINCHUNKSIZE;
}

static void checker()
{
    printf("---------------\n");
    printf("blocks:\n");
    for (void *p = heap + (CLASSNUM + 1) * WSIZE; get_size(p); p += get_size(p))
        printf("%p\t%d\t%d\n", p, get_size(p), get_alloc(p));
    printf("---------------\n");
}
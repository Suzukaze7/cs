#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

void add(struct buf *prev, struct buf *next);
void remove(struct buf *b);

struct bucket
{
  struct buf head;
  struct spinlock lock;
};

struct
{
  struct spinlock lock;
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // Sorted by how recently the buffer was used.
  // head.next is most recent, head.prev is least.
  // struct buf head;

  struct bucket table[NTBL];
} bcache;

void
binit(void)
{
  struct buf *head;

  initlock(&bcache.lock, "bcache");

  // init table
  char s[] = "bcache table0";
  static char name[NTBL][14];
  for (int i = 0; i < NTBL; i++)
  {
    strncpy(name[i], s, sizeof s);
    name[i][12] += i;
    initlock(&bcache.table[i].lock, name[i]);

    head = &bcache.table[i].head;
    head->next = head->prev = head;
  }

  // Create hash table of buffers
  head = &bcache.table[0].head;
  for (struct buf *b = bcache.buf; b < bcache.buf + NBUF; b++)
  {
    add(head, b);
    initsleeplock(&b->lock, "buffer");
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf *
bget(uint dev, uint blockno)
{
  struct buf *b;
  struct bucket *bucket = &bcache.table[blockno % NTBL];

  // Is the block already cached?
  acquire(&bucket->lock);
  for (b = bucket->head.next; b != &bucket->head; b = b->next)
  {
    if (b->dev == dev && b->blockno == blockno)
    {
      b->refcnt++;
      b->ticks = get_ticks();
      release(&bucket->lock);
      acquiresleep(&b->lock);
      return b;
    }
  }
  release(&bucket->lock);

  // Not cached.
  // Recycle the least recently used (LRU) unused buffer.
  struct buf *sel = 0;
  struct spinlock *lock = 0;
  acquire(&bcache.lock);
  for (bucket = bcache.table; bucket < bcache.table + NTBL; bucket++)
  {
    acquire(&bucket->lock);
    for (b = bucket->head.next; b != &bucket->head; b = b->next)
    {
      if (b->refcnt == 0 && (!sel || b->ticks < sel->ticks))
      {
        if (&bucket->lock != lock)
        {
          if (lock)
            release(lock);
          lock = &bucket->lock;
        }

        sel = b;
      }
    }

    if (lock != &bucket->lock)
      release(&bucket->lock);
  }
  release(&bcache.lock);

  if (sel)
  {
    remove(sel);
    release(lock);

    bucket = &bcache.table[blockno % NTBL];
    acquire(&bucket->lock);
    add(&bucket->head, sel);
    sel->dev = dev;
    sel->blockno = blockno;
    sel->valid = 0;
    sel->refcnt = 1;
    b->ticks = get_ticks();
    release(&bucket->lock);

    acquiresleep(&sel->lock);
    return sel;
  }

  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf *
  bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if (!b->valid)
  {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if (!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if (!holdingsleep(&b->lock))
    panic("brelse");

  bunpin(b);

  releasesleep(&b->lock);
}

void
bpin(struct buf *b)
{
  struct bucket *bucket = &bcache.table[b->blockno % NTBL];
  acquire(&bucket->lock);
  b->refcnt++;
  release(&bucket->lock);
}

void
bunpin(struct buf *b)
{
  struct bucket *bucket = &bcache.table[b->blockno % NTBL];
  acquire(&bucket->lock);
  b->refcnt--;
  release(&bucket->lock);
}

void add(struct buf *prev, struct buf *next)
{
  next->next = prev->next;
  next->prev = prev;
  prev->next = prev->next->prev = next;
}

void remove(struct buf *b)
{
  b->prev->next = b->next;
  b->next->prev = b->prev;
}

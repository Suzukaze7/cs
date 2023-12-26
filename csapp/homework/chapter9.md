- 9.1

|8|2^8 = 256|2^8 - 1 = 255|
|---|---|---|
|16|2^16 = 64k|2^16 - 1 = 64k - 1|
|32|2^32 = 4G|2^32 - 1 = 4G - 1|
|48|2^48 = 256T|2^48 - 1 = 256T - 1|
|64|2^64 = 16E|2^64 - 1 = 16E - 1|

- 9.2
```
2^(16 - 12) = 2^4 = 16
2^(16 - 13) = 2^3 = 8
2^(32 - 12) = 2^20 = 1M
2^(32 - 13) = 2^19 = 512K
```

- 9.3

|22|10|14|10|
|---|---|---|---|
|21|11|13|11|
|20|12|12|12|
|19|13|11|13|

- 9.4
```
A: 00001111010111
B:
0xf
0x3
0x3
是
否
0xd
C: 001101010111
D:
0x17
0x1
0x3
否
-

XXX
D:
0x3
0x5
0xd
是
0x1d
```

- 9.5
```cpp
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    struct stat stat;
    fstat(fd, &stat);

    void *buf = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    write(STDOUT_FILENO, buf, stat.st_size);

    munmap(buf, stat.st_size);
}
```

- 9.6

|8|0x9|
|---|---|
|8|0x9|
|16|0x11|
|16|0x11|

XXX
|-|-|
|---|---|
|16|0x11|
|-|-|
|24|0x19|
// 头部为 4 字节

- 9.7
```
12
8
16
8
```

- 9.8
```cpp
static void *find_fit(size_t asize)
{
    for (void *bp = heap_listp; GET_SIZE(HDRP(bp)); bp = NEXT_BLKP(bp))
        if (!GET_ALLOC(HDRP(bp)) && GET_SIZE(HDRP(bp)) >= asize)
            return bp;
    return NULL;
}
```

- 9.9
```cpp
static void place(void *bp, size_t asize)
{
    size_t psize = GET_SIZE(HDRP(bp));
    if (psize - asize >= 2 * DSIZE)
    {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));

        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(psize - asize, 0));
        PUT(FTRP(bp), PACK(psize - asize, 0));
    }
    else
    {
        PUT(HDRP(bp), PACK(psize, 1));
        PUT(FTRP(bp), PACK(psize, 1));
    }
}
```

- 9.10
```
先请求大量最小类的块并释放，再请求更大一级类的块并释放，以此重复
```
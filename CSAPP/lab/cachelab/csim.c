// author suzukaze

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>

#include "cachelab.h"

char help_str[] = "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n\
Options:\n\
  -h         Print this help message.\n\
  -v         Optional verbose flag.\n\
  -s <num>   Number of set index bits.\n\
  -E <num>   Number of lines per set.\n\
  -b <num>   Number of block offset bits.\n\
  -t <file>  Trace file.\n\
\n\
Examples:\n\
  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n\
  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace";

struct Line
{
    int valid, tag, last;
};

int main(int argc, char **argv)
{
    int ver = 0, setb = 0, line = 0, offb = 0, opt;
    char *trace = NULL;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("%s", help_str);
            return 0;
        case 'v':
            ver = 1;
            break;
        case 's':
            setb = atoi(optarg);
            break;
        case 'E':
            line = atoi(optarg);
            break;
        case 'b':
            offb = atoi(optarg);
            break;
        case 't':
            trace = optarg;
            break;
        default:
            printf("%s", help_str);
            return 0;
        }
    }

    int setc = 1 << setb;
    struct Line **cache = malloc(sizeof(struct Line *) * setc);
    for (int i = 0; i < setc; i++)
    {
        cache[i] = malloc(sizeof(struct Line) * line);
        for (int j = 0; j < line; j++)
            cache[i][j].valid = 0;
    }

    int hit = 0, miss = 0, evict = 0;
    char buf[1024];
    FILE *fd = fopen(trace, "r");
    for (int cur = 1; 1; cur++)
    {
        char op;
        unsigned addr, size;
        if (fscanf(fd, "%s %x,%u", &op, &addr, &size) == EOF)
            break;

        if (op == 'I')
            continue;

        sprintf(buf, "%c %x,%u", op, addr, size);

        int tset = addr >> offb & ((1 << setb) - 1), tag = addr >> (offb + setb), flag = 0;
        struct Line *target = cache[tset];
        for (int i = 0; i < line; i++)
            if (target[i].valid && target[i].tag == tag)
            {
                target[i].last = cur;
                flag = 1;
                break;
            }

        if (flag)
        {
            hit++;
            if (ver)
                printf("%s hit\n", buf);

            if (op == 'M')
            {
                hit++;
                if (ver)
                    printf("%s hit\n", buf);
            }
            continue;
        }

        miss++;
        if (ver)
            printf("%s miss", buf);

        int last = 0x3f3f3f3f, select;
        flag = 0;
        for (int i = 0; i < line; i++)
            if (!target[i].valid)
            {
                select = i;
                flag = 1;
                break;
            }
            else if (target[i].last < last)
            {
                last = target[i].last;
                select = i;
            }

        if (!flag)
        {
            evict++;
            if (ver)
                printf(" eviction");
        }
        target[select].tag = tag;
        target[select].last = cur;
        target[select].valid = 1;

        if (op == 'M')
        {
            hit++;
            if (ver)
                printf(" hit");
        }

        if (ver)
            puts("");
    }

    printSummary(hit, miss, evict);

    return 0;
}
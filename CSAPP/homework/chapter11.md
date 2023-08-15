- 11.1

|-|0.0.0.0|
|---|---|
|-|255.255.255.255|
|-|127.0.0.1|
|0xcdbca079|-|
|0x400c950d|-|
|0xcdbc9217|-|

- 11.2
```cpp
#include<stdio.h>

int main(int argc, char **argv)
{
    unsigned n;
    sscanf(argv[1], "%x", &n);
    printf("%u\n", n);
    for (int i = 3; ~i; i--)
    {
        printf("%d", n >> i * 8 & (1 << 8) - 1);
        if (i)
            printf(".");
    }
}
```

- 11.3
```cpp
#include<stdio.h>

int main(int argc, char **argv)
{
    int res = 0;
    int a[4];
    sscanf(argv[1], "%d.%d.%d.%d", a, a + 1, a + 2, a + 3);
    for (int i = 0; i < 4; i++)
        res = (res << 8) + a[i];
    printf("0x%x", res);
}
```
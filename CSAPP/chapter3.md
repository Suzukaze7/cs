- 3.5
```cpp
void decode(long *xp, long *yp, long *zp)
{
    long t1 = *xp, t2 = *yp, t3 = *zp;
    *yp = t1, *zp = t3, *xp = t1;
}
```

- 3.7
```cpp
long scale2(long x, long y, long z)
{
    long t = 5 * x + 2 * y + 8 * z;
    return t;
}
```

- 3.10
```cpp
long arith2(long x, long y, long z)
{
    long t1 = x | y;
    long t2 = t1 >> 3;
    long t3 = -t2;
    long t4 = z - t3;
    return t4;
}
```

- 3.16
```cpp
void cond(long a, long *p)
{
    if (!p)
        goto L1;
    if (*p >= a)
        goto L1;
    *p = a;
L1:
    return;
}
```

- 3.17
```cpp
long lt_cnt = 0;
long ge_cnt = 0;
long absdiff_se(long x, long y)
{
    long result;
    if (x < y)
        goto x_lt_y;
    ge_cnt++;
    result = x - y;
    goto done;
x_lt_y:
    lt_cnt++;
    result = y - x;
done:
    return result;
}
```

- 3.18
```cpp
long test(long x, long y, long z)
{
    long val = x + y + z;
    if (x < -3)
    {
        if (y < z)
            val = x * y;
        else
            val = y * z;
    }
    else if (x > 2)
        val = x * z;
    return val;
}
```

- 3.21
```cpp
long test(long x, long y)
{
    long val = 8 * x;
    if (y > 0)
    {
        if (x < y)
            val = y - x;
        else
            val = x & y;
    }
    else if (y <= -2)
        val = x + y;
    return val;
}
```

- 3.24
```cpp
long loop_while(long a, long b)
{
    long result = 1;
    while (a < b)
    {
        result *= a + b;
        a++;
    }
   
   return result;
}
```

- 3.25
```cpp
long loop_while2(long a, long b)
{
    long result = b;
    while (b > 0)
    {
        result *= a;
        b -= a;
    }
    return result;
}
```

- 3.27
```cpp
long fact_for_while(long n)
{
    long result = 1;
    long i = 2;
    while (i <= n)
    {
        result *= i;
        i++;
    }
    return result;
}
long fact_for_goto(long n)
{
    long result = 1;
    long i = 2;
    if (i > n)
        goto done;
loop:
    result *= i;
    i++;
    if (i <= n)
        goto loop;
done:
    return result;
}
```

- 3.28
```cpp
long fun_b(unsigned long x)
{
    long val = 0;
    long i;
    for (i = 64; i; i--)
    {
        long t = x & 1;
        val += val;
        val |= t;
        x >>= 1;
    }
    return val;
}
```

- 3.29
```cpp
void func3_29_while()
{
    long sum = 0;
    long i = 0;
    while (i < 10)
    {
        if (i & 1)
            continue;
        sum += i;
        i++;
    }
}
void func3_29_goto()
{
    long sum = 0;
    long i = 0;
loop:
    if (i & 1)
        goto update;
    sum += i;
update:
    i++;
    if (i < 10)
        goto loop;
}
```
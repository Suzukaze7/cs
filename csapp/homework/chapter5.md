- 5.1
```
xp指向的值永远为0
```

- 5.2
```
1: n <= 2
2: 3 <= n <= 7
3: 8 <= n
```

- 5.3

|代码|min|max|incr|square|
|----|----|----|----|----|
|A|1|91|90|90|
|B|91|1|90|90|
|C|1|1|90|90|

- 5.4
```
A: 没有什么不同
B: 实现了
C: 因为每次循环开始 %xmm0 的值都等于将要取出来的 dest 的值
```

- 5.5
```
A: 2 * degree 次加法与乘法
B: 循环变量为 i 和 xpwr，而 xpwr 为浮点数乘，i 为整数加，所以 xpwr 的计算成为性能瓶颈，所以延迟界限为 5

XXX
A: degree 次加法与 2 * degree 次乘法 //i 不算循环内
B: xpwr 与 result 进行比较，xpwr 为限制因素
```

- 5.6
```
A: degree 次加法与乘法
B: 每次循环需要顺序执行的加法与乘法，所以延迟为和 8
C: 5.5的函数更好地利用了指令级并行
```

- 5.7
```cpp
void combine5(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 5;
    data_t = acc = IDENT;

    for (i = 0; i < limit; i += 5)
        acc = acc OP data[i] OP data[i + 1] OP data[i + 2] OP data[i + 3] OP data[i + 4];
    
    for (i < length; i++)
        acc = acc OP data[i];
}
```

- 5.8
```
15, 10, 5, 5, 10 ?

XXX
5, 3.33, 1.67, 1.67, 3.33
```

- 5.9
```cpp
void merge(long src1[], long src2[], long dest[], long n)
{
    long i1 = 0;
    long i2 = 0;
    long id = 0;
    while (i1 < n && i2 < n)
    {
        long t1 = src1[i1 + 1], t2 = src2[i2 + 1];
        dest[id++] = t1 < t2 ? t1 : t2;
        t1 += t1 < t2, t2 += !(t1 < t2);

        XXX
        long t1 = src1[i1 + 1], t2 = src2[i2 + 1], t3 = t1 < t2;
        dest[id++] = t3 ? t1 : t2;
        t1 += 3, t2 += !t3;
    }
}
```

- 5.10
```
A: 将 a[0 - 998] 赋值为后一个元素
B: 将数组所有元素赋值成 a[0]
C: 因为 A 的数据相关只有 i，而 B 的数据相关有数组元素的存取
D: 是 A 的两倍

XXX
D: 即使每一次循环需要顺序存取，但每次循环之间没有数据相关，所以可以并发执行
```

- 5.11
```
s_data -> add -> load -> add -> ...
因为存在写/读相关
```

- 5.12
```cpp
void psum1(float a[], float p[], long n)
{
    long i
    float sum;
    sum = p[0] = a[0];
    for (i = 1; i < n; i++)
    {
        sum += a[i];
        p[i] = sum;
    }
}
```
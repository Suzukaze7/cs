- 7.1

|是|外部|m.o|.data|
|---|---|---|---|
|是|外部|swap.o|.data|
|是|外部|swap.o|COMMON|
|是|外部|swap.o|.text|
|否|-|-|-|

XXX
|-|-|-|-|
|---|---|---|---|
|-|全局|-|-|
|-|全局|-|-|
|-|全局|-|-|
|-|-|-|-|

// 只有实际被其他目标文件引用了才算外部

- 7.2
```
A:
(a)main.1
(b)main.2

B:
(a)main.1
(b)main.2

C:
(a)x.2
(b)x.2

XXX
B: 这是一个错误

// 也就是说只有重载时才会发生重组？
```

- 7.3
```
A: gcc p.o libx.a
B: gcc p.o libx.a liby.a
C: gcc p.o libx.a liby.a libx.a p.o

XXX
C: gcc p.o libx.a liby.a libx.a
// .o文件会把所有定义与引用加入集合，所以不需要重复
// 而.a文件只会把需要的给加入集合，所以只有在保证 p.o 和 liby.a 所引用的是相同符号才不需要再次把 libx.a 加入集合
```

- 7.4
```
A: 0x4004e8
B: 0x5

XXX
A: 0x4004df
```

- 7.5
```
ADDR(s) = ADDR(.text) = 0x4004d0
ADDR(r.symbol) = ADDR(swap) = 0x4004e8
refaddr = ADDR(s) + r.offset = 0x4004d0 + 0xa = 0x4004da
*refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr)
        = (unsigned) (0x4004e8 + (-4) - 0x4004da)
        = (unsigned) (0xa)
```
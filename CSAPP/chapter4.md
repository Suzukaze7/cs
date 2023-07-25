- 4.3
```asm
# long sum(long *start, long count)
# start in %rdi, count in %rsi
sum:
    xorq %rax,%rax
    andq %rsi,%rsi
    jmp test
loop:
    mrmovq (%rdi),%r8
    addq %r8,%rax
    iaddq $8,%rdi
    iaddq $-1,%rsi
test:
    jne loop
    ret
```

- 4.4
```asm
# long rsum(long *start, long count)
# start in %rdi, count in %rsi
rsum:
    andq %rsi,%rsi
    jg .L1
    irmovq $0,%rax
    ret
.L1:
    pushq %rdi
    irmovq $8,%r8
    irmovq $1,%r9
    addq %r8,%rdi
    subq %r9,%rsi
    call rsum
    popq %rdi
    mrmovq (%rdi),%rdi
    addq %rdi,%rax
    ret
```

- 4.5
```asm
# long absSum(long *start, long count)
# start in %rdi, count in %rsi
sum:
    irmovq $8,%r8
    irmovq $1,%r9
    xorq %rax,%rax
    andq %rsi,%rsi
    jmp test
loop:
    mrmovq 0(%rdi),%r10
    andq %r10,%r10
    jl neg
    addq %r10,%rax
    jmp done
neg:
    subq %r10,%rax
done:
    addq $r8,%rdi
    subq $r9,%rsi
test:
    jne loop
    ret
```

- 4.6
```asm
# long absSum(long *start, long count)
# start in %rdi, count in %rsi
sum:
    irmovq $8,%r8
    irmovq $1,%r9
    xorq %rax,%rax
    andq %rsi,%rsi
    jmp test
loop:
    mrmovq (%rdi),%r10
    subq %r10,%r11
    xorq %r11,%r11
    cmovg %r11,%r10
    addq %r10,%rax
    addq $r8,%rdi
    subq $r9,%rsi
test:
    jne loop
    ret
```

- 4.9
```
bool xor = (!a && b) || (a && !b);
```

- 4.11
```
word out = [
    A <= B && A <= C : A;
    B <= C : B;
    1 : C;
];
```

- 4.12
```
word out = [
    B <= A && A <= C || C <= A && A <= B : A;
    A <= B && B <= C || C <= B && B <= A : B;
    1 : C;
];
```

- 4.19
```
bool need_valC =
        icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ,
                    IJXX, ICALL };
```

- 4.20
```
word srcB = [
        icode in { IRMMOVQ, IMRMOVQ, IOPQ } : rB;
        icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;
];
```

- 4.21
```
word dstM = [
        icode in { IMRMOVQ, IPOPQ } : rA;
        1 : RNONE;
];
```

- 4.22
```
E端口
```

- 4.23
```
word aluB = [
        icode in { IOPQ, IMRMOVQ, IRMMOVQ, IPUSHQ, IPOPQ, IRET } : valB;
        icode in { IIRMOVQ, IRRMOVQ } : 0;
];
```

- 4.24
```
XXX
word dstE = [
        icode in { IRRMOVQ } && Cnd : rB;
        icode in { IIRMOVQ, IOPQ } : rB;
        icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;
]
```

- 4.25
```
word mem_data = [
        icode in { IRMMOVQ, IPUSHQ } : valA;
        icode in { ICALL } : valP;
]
```

- 4.26
```
bool mem_write = icode in { IRMMOVQ, IPUSHQ, ICALL };
```

- 4.27
```
bool SAOK = !imem_error $$ instr_valid && !dmem_error;
bool SADR = imem_error || dmem_error;
bool SINS = instr_valid;
bool SHLT = icode == IHALT;

XXX
word Stat = [
        imem_error || dmem_error : SADR;
        !instr_valid : SINS;
        icode == IHALT : SHLT;
        1 : SAOK;
]
```

- 4.28
```
A: C之后; 2 / (340 * 1e-12) = 5.88GIPS; 340ps;
B: B,D之后; 3 / (360 * 1e-12) = 8.33GIPS; 360ps;
C: A,C,D之后; 4 / (380 * 1e-12) = 10.53GIPS; 380ps;

XXX
A: C之后; 1 / (190 * 1e-12) = 5.26GIPS; 340ps;
B: B,D之后; 1 / (130 * 1e-12) = 7.69GIPS; 360ps;
C: A,C,D之后; 1 / (110 * 1e-12) = 9.09GIPS; 380ps;

// 忽略填满流水线的过程，相当于是一个周期执行一条完整指令
```

- 4.29
```
A:
delay(k) = (300 / k + k) * 1e-12ps
thruput(k) = 1 / ((300 / k + k) * 1e-12)GIPS;
B:
kmax = sqrt(300) = 17
thruput(17) = 28.86GIPS

XXX
delay(k) = (300 / k + 20k)ps
thruput(k) = 1 / ((300 / k + 20k) * 1e-12)GIPS;
B: 当k趋近于无穷大，吞吐量为 50GIPS
```

- 4.30
```
f_stat = [
        imem_error : SADR;
        !instr_valid : SINS;
        icode == IHALT : SHLT;
        1 : SAOK;
]
```

- 4.31
```
XXX
word d_dstE = [
        icode in { IRRMOVQ, IIRMOVQ, IOPQ } : D_rB;
        icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
        1 : RNONE;
]

// P293: 作为一条通用原则，我们要保存处于一个流水线阶段中的指令的所有信息。
```

- 4.33
```asm
irmovq $5,%rdx
irmovq $0x100,%rsp
rmmovq %rdx,0(%rsp)
popq %rsp
nop
nop
rrmovq %rsp,%rax
```

- 4.34
```
word d_valB = [
    d_srcB == e_dstE : e_valE;
    d_srcB == M_dstM : m_valM;
    d_srcB == M_dstE : M_valE;
    d_srcB == W_dstM : W_valM;
    d_srcB == W_dstE : W_valE;
    1 : d_rvalB;
]
```

- 4.35
```asm
irmovq $1,%rbx
irmovq $2,%rdx
xorq %rax,%rax
cmovne %rbx,%rdx
addq %rdx,%rdx
```

- 4.36
```
word m_stat = [
    M_stat != SAOK : M_stat;
    dmem_error : SADR;
    1 : SAOK;
]

XXX
word m_stat = [
    dmem_error : SADR;
    1 : M_stat;
]
```

- 4.37
```asm
    call .L1
.L1:
    xorq %rax,%rax
    jne .L1
    ret

XXX
    xorq %rax,%rax
    jne .L1
    irmovq $1,%rax
.L1:
    ret
```

- 4.38
```asm
irmovq $0x100,%rbx
mrmovq 0(%rbx),%rsp
ret
halt
```

- 4.43
```
CPI = 1 + 1 * 0.25 * 0.2 + 2 * 0.2 * 0.35 + 3 * 0.02 = 1.25
```

- 4.44
```
A:
Ci1 = (7 + 8) / 2 * n + 7 = 7.5 * n + 7
Ci2 = 8 * n + 7
B:
Cb1 = n + n * 2 * 0.5 = 2 * n
Cb2 = n
C:
CPI1 = 1 + 2 * n / (7.5 * n + 7)
CPI2 = 1 + n / (8 * n + 7)
```
- 8.1
```
是
否
是
```

- 8.2
```
A: 2
B: 0
```

- 8.3
```
acbc
abcc
bacc
bcac

XXX
bcac 不可能，因为waitpid的存在
```
- 8.4
```
6行
0
Bye
Hello
1
2
Bye
```

- 8.5
```cpp
unsigned int snooze(unsigned int secs)
{
    unsigned int t = sleep(secs);
    printf("Slept for %d of %d secs.\n", secs - t, secs);
    return t;
}
```

- 8.6
```cpp
#include<stdio.h>

int main(int argc, char **argv, char **envp)
{
    printf("Command-ine arguments:\n");
    for (int i = 0; i < argc; i++)
        printf("argv[%2d]: %s\n", i, argv[i]);
    printf("Environment variables:\n");
    for (int i = 0; envp[i]; i++)
        printf("envp[%2d]: %s\n", i, envp[i]);
}
```

- 8.7
```cpp
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

unsigned int snooze(unsigned int secs)
{
    unsigned int t = sleep(secs);
    printf("Slept for %d of %d secs.\n", secs - t, secs);
    return t;
}

void sigint_handler(int sig) { }

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);
    snooze(atoi(argv[1]));
}
```

- 8.8
```
2
1
2

XXX
212
// 因为两个进程不共享全局变量
```
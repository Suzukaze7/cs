#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int main(int argc, char *argv[])
{
    int fd[2][2];
    pipe(fd[0]), pipe(fd[1]);

    int pid = fork();
    if (pid)
    {
        char send[] = "ping";
        write(fd[0][1], send, strlen(send));
        close(fd[0][0]), close(fd[0][1]);

        char recv[5];
        read(fd[1][0], recv, sizeof recv);
        close(fd[1][0]), close(fd[1][1]);

        printf("%d: received %s\n", getpid(), recv);
    }
    else
    {
        char recv[5];
        read(fd[0][0], recv, sizeof recv);
        close(fd[0][0]), close(fd[0][1]);

        printf("%d: received %s\n", getpid(), recv);

        char send[] = "pong";
        write(fd[1][1], send, strlen(send));
        close(fd[1][0]), close(fd[1][1]);
    }
    exit(0);
}

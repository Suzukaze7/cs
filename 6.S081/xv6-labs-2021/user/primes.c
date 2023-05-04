#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

void child_thread_op(int *left)
{
    close(left[1]);

    int prime;
    if (read(left[0], &prime, sizeof(int)))
    {
        printf("prime %d\n", prime);

        int pid, right[2];
        pipe(right);
        pid = fork();

        if (pid)
        {
            close(right[0]);

            while (read(left[0], &prime, sizeof(int)))
                write(right[1], &prime, sizeof(int));

            close(right[1]);
            wait(0);
        }
        else
            child_thread_op(right);
    }

    close(left[0]);
    exit(0);
}

int main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid)
    {
        close(fd[0]);

        for (int i = 2; i <= 35; i++)
        {
            int flag = 1;
            for (int j = 2; j <= i / j; j++)
                if (i % j == 0)
                {
                    flag = 0;
                    break;
                }

            if (flag)
                write(fd[1], &i, sizeof(int));
        }

        close(fd[1]);
        wait(0);
        exit(0);
    }
    else
    {
        child_thread_op(fd);
        exit(0);
    }
}

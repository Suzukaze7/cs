#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char *err = "usage: sleep [number]\n";
        write(1, err, strlen(err));
        exit(1);
    }

    sleep(atoi(argv[1]));
    exit(0);
}

#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/fs.h"

#define MAX_SIZE 1024

void find(char *path, char *tar)
{
    int fd = open(path, 0);
    struct stat st;
    struct dirent de;

    char *p = path + strlen(path);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        strcpy(p, de.name);
        if (!strcmp(tar, de.name))
            printf("%s\n", path);

        stat(path, &st);
        if (st.type == T_DIR)
            find(path, tar);
    }

    close(fd);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage find [dir] [file]\n");
        exit(1);
    }

    struct stat st;
    stat(argv[1], &st);
    if (st.type == T_FILE)
    {
        printf("usage find [dir] [file]\n");
        exit(1);
    }

    char path[MAX_SIZE];
    strcpy(path, argv[1]);
    find(path, argv[2]);

    exit(0);
}

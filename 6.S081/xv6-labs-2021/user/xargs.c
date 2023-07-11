#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

#define stdin 0
#define N 128
#define LEN 10

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("usage: xargs [command] [param...]\n");
		exit(1);
	}

	char *parg[N], arg[N][LEN];
	int pidx = 0;
	for (int i = 1; i < argc; i++)
		parg[pidx++] = argv[i];

	char buf;

	int arg_idx = 0, arg_len = 0;
	while (read(stdin, &buf, 1))
	{
		if ((buf == ' ' || buf == '\n') && arg_len)
		{
			parg[pidx++] = arg[arg_idx++];
			arg_len = 0;
		}
		else
			arg[arg_idx][arg_len++] = buf;
	}

	int pid = fork();
	if (pid)
		wait(0);
	else
		exec(argv[1], parg);

	exit(0);
}

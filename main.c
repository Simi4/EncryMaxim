#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/wait.h>
#include "common.h"


void help_usage(void)
{
	puts("Image filter");
	puts("Example use:");
	puts("./bin --in file1.txt --of file2.txt --key key.txt");
}


int main(int argc, char *argv[])
{
	struct option long_opt[] = {
		{"in",   1, 0, 'i'},
		{"out",  1, 0, 'o'},
		{"key",  1, 0, 'k'},
		{"help", 0, 0, 'h'},
		{0,      0, 0, 0  }
	};

	char *inpath, *outpath, *keypath;

	inpath = outpath = keypath = NULL;

	while (1) {
		int c = getopt_long(argc, argv, "i:o:k:h", long_opt, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'i':
			inpath = optarg;
			break;

		case 'o':
			outpath = optarg;
			break;

		case 'k':
			keypath = optarg;
			break;

		default:
			help_usage();
			exit(EXIT_SUCCESS);
		}
	}

	if (!inpath || !outpath || !keypath) {
		help_usage();
		exit(EXIT_SUCCESS);
	}

	int in_pipe_fd[2], key_pipe_fd[2];

	assert(pipe(in_pipe_fd) != -1);
	assert(pipe(key_pipe_fd) != -1);

	pid_t pid1, pid2;

	pid1 = safe_fork();
	if (pid1 == 0) {
		// child 1
		safe_close(1); // close read
		assert(dup(in_pipe_fd[1]) == 1);
		safe_close(in_pipe_fd[1]);
		safe_close(in_pipe_fd[0]);
		safe_close(key_pipe_fd[1]);
		safe_close(key_pipe_fd[0]);
		execlp("cat", "cat", inpath, (char*)0);
		_exit(EXIT_SUCCESS);
	} else {
		pid2 = safe_fork();
		if (pid2 == 0) {
			//child 2
			safe_close(1);
			assert(dup(key_pipe_fd[1]) == 1);
			safe_close(in_pipe_fd[1]);
			safe_close(in_pipe_fd[0]);
			safe_close(key_pipe_fd[1]);
			safe_close(key_pipe_fd[0]);
			execlp("cat", "cat", keypath, (char*)0);
			_exit(EXIT_SUCCESS);
		} else {
			safe_close(in_pipe_fd[1]);
			safe_close(key_pipe_fd[1]);

			int out_fd = safe_creat(outpath, 0700);

			while (1) {
				char c1, c2, res;

				if (read(in_pipe_fd[0], &c1, 1) != 1)
					break;

				if (read(key_pipe_fd[0], &c2, 1) != 1)
					break;

				res = c1 ^ c2;
				safe_write(out_fd, &res, 1);
			}

			safe_close(in_pipe_fd[0]);
			safe_close(key_pipe_fd[0]);
			safe_close(out_fd);
		}
	}

	exit(EXIT_SUCCESS);
}

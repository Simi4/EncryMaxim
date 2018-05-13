#include "common.h"
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>


pid_t safe_fork(void)
{
	pid_t pid = fork();

	assert(pid != -1);
	return pid;
}

int safe_creat(const char *pathname, mode_t mode)
{
	int fd = creat(pathname, mode);

	assert(fd != -1);
	return fd;
}

int safe_open(const char *pathname, int flags)
{
	int fd = open(pathname, flags);

	assert(fd != -1);
	return fd;
}

void safe_read(int fd, void *buf, size_t count)
{
	assert(read(fd, buf, count) == count);
}

void *safe_calloc(size_t size, size_t cnt)
{
	void *buf = calloc(size, cnt);

	assert(buf != NULL);
	return buf;
}

void *safe_malloc(size_t size)
{
	void *buf = malloc(size);

	assert(buf != NULL);
	return buf;
}

void safe_free(void *buf)
{
	assert(buf != NULL);
	free(buf);
}

void safe_write(int fd, const void *buf, size_t count)
{
	assert(write(fd, buf, count) == count);
}

void safe_close(int fd)
{
	assert(close(fd) == 0);
}

#pragma once
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>


pid_t safe_fork(void);

int safe_creat(const char *pathname, mode_t mode);

int safe_open(const char *pathname, int flags);

void safe_read(int fd, void *buf, size_t count);

void *safe_malloc(size_t size);

void *safe_calloc(size_t size, size_t cnt);

void safe_free(void *buf);

void safe_write(int fd, const void *buf, size_t count);

void safe_close(int fd);

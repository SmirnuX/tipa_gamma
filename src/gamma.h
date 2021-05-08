#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>

#define PERM 0666

int duplicate(char* program, int* _pipe);

char **string_parser(char *string, char *delim);
char *new_str_copy(char *source, int beginning, int end);

void close_pipes(int** pipes);
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define POINTER_INIT 0
#define COUNTER_INIT 0

/*
utils.c is a libdryox module.
It serves the following extern functions:
    void mkdirp(char *dirpath);

It defines the following types:

It allocates the following variables:

*/

// mkdirp version 0.2.0
int mkdirp(char *dirpath)
{
    char path[PATH_MAX];
    strncpy(path, dirpath, PATH_MAX);

    for (size_t i = 1; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            path[i] = '\0';
            if (mkdir(path, 0755) != 0 && errno != EEXIST)
                return -1;
            path[i] = '/';
        }
    }
    return (mkdir(dirpath, 0755) == 0 || errno == EEXIST) ? 0 : -1;
}

#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define POINTER_INIT 0
#define COUNTER_INIT 0

/*
Utils.c is a libdryox module.
It serves the following extern functions:
    void mkdirp(char *dirpath);

It defines the following types:

It allocates the following variables:

*/

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

/*
int mkdirp(char *dirpath)
{
    size_t c, slash_count = COUNTER_INIT;
    char path[PATH_MAX];
    strncpy(path, dirpath, PATH_MAX);

    while (mkdir(dirpath, 0755) != 0)
    {
        for (c = POINTER_INIT; path[c] != '\0'; c++)
            ; // set c to '\0'...
        c--;  // ...and then point it to the character before that.

        if (mkdir(path, 0755) != 0)
        {
            for (; path[c] != '/'; c--)
                ;             // set c to '/'...
            path[c] = '\0';   // ...and end the string there...
            slash_count++;    // ...while noting that a string was nullified...
            c = POINTER_INIT; // ...and, as a guard, reinitialize c.
        }

        else if (slash_count > 0)
        {
            for (c = POINTER_INIT; path[c] != '\0'; c++)
                ;          // set c to '\0'...
            path[c] = '/'; // ...and change the terminator to a slash.
        }
    }

    return 0;
}
*/
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
    /*
     * mkdirp is a reimplementation of the shell mkdir -p.
     *
     * This implementation of it works by:
     *  Does the directory exist?
     *  If not, can it be made?
     *  If not, create a new allocated string with a UID based on the pointer.
     *      Does the directory's parent exist?
     *      If not, can it be made?...
     *
     *  Once the earliest parent is found, walk and mkdir the path.
     *
     */

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
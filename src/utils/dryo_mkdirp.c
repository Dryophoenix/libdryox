#include "utils/dryo_mkdirp.h"
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define POINTER_INIT 0
#define COUNTER_INIT 0

/*
dryo_mkdirp.c is a libdryox module.

It serves the following extern functions:
  int dryo_mkdirp(const char *dirpath);

It defines the following types:
  (none)
*/

int dryo_mkdirp(const char *dirpath)
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

#ifndef DRYOTU_ENVUTILS_H
#define DRYOTU_ENVUTILS_H

#define CACHESETENV(name, buf, value) ((buf) = getenv(name), setenv(name, value, 1))

#define RESTOREENV(name, buf) ((buf) != NULL ? setenv(name, buf, 1) : unsetenv(name))

#endif
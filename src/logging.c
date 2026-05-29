#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dryox/init.h"

#define DRYOX_TIMESTAMP_SIZE 32

// dryox/init.h prefers this metadata.
static char PROJECT_NAME[] = "DryoX/libdryox";
static char FILE_NAME[] = "logging";
__attribute__((constructor)) static void init(void)
{
    dryoinit(FILE_NAME, PROJECT_NAME);
}

/*
Logging.c is a libdryox module.
It serves the following extern functions:
    dryolog(Level level, char * message, ...);

It defines the following types:
    typedef enum Log_Level;
*/

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
} Log_Level;

static char LOG_PATH[PATH_MAX];

int dryolog(Log_Level level, char *format, ...)
{
    // make a timestamp ts with current time since unix epoch
    char ts[DRYOX_TIMESTAMP_SIZE];
    time_t now = time(NULL);
    strftime(ts, sizeof(ts), "%m-%d-%Y %H:%M:%S", localtime(&now));

    // open file
    FILE *logfile = fopen(LOG_PATH, "a");
    if (logfile == NULL)
    {
        fprintf(stderr,
                "The libdryox module \"logging\" failed to open the log file.\n"
                "FILE logfile may be undefined,\n"
                "or logging.h has no access to it.\n"
                "exiting logging as fatal.\n");
        return -1;
    }

    // start variadics
    va_list args;
    va_list args_for_error;
    va_start(args, format);
    va_copy(args_for_error, args);

    // handle each level
    switch (level)
    {

    case LOG_DEBUG:
        fprintf(logfile, "[%s] [DEBUG] ", ts);
        vfprintf(logfile, format, args);
        fprintf(logfile, "\n");
        break;

    case LOG_INFO:
        fprintf(logfile, "[%s] [INFO] ", ts);
        vfprintf(logfile, format, args);
        fprintf(logfile, "\n");
        break;

    case LOG_WARN:
        fprintf(logfile, "[%s] [WARN] ", ts);
        vfprintf(logfile, format, args);
        fprintf(logfile, "\n");
        break;

    case LOG_ERROR:
        fprintf(logfile, "[%s] [ERROR] ", ts);
        vfprintf(logfile, format, args);
        vfprintf(stderr, format, args_for_error);
        fprintf(logfile, "\n");
        break;

    case LOG_FATAL:
        fprintf(logfile, "[%s] [FATAL] ", ts);
        vfprintf(logfile, format, args);
        vfprintf(stderr, format, args_for_error);
        fprintf(logfile, "\n");
        break;
    }

    // end variadics
    va_end(args);
    va_end(args_for_error);

    // close file
    fclose(logfile);

    return 0;
}
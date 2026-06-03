#include "dryox/dryologging.h"
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dryox/init.h"

/*
dryologging.c is a libdryox module.
It serves the following extern functions:
    dryolog(Level level, char * message, ...);

It defines the following types:
    typedef enum Log_Level;
*/

#define DRYOX_TIMESTAMP_SIZE 32

static char PROJECT_NAME[] = "DryoX/libdryox";
static char FILE_NAME[] = "logging";
static char LOG_FILE[PATH_MAX];
__attribute__((constructor)) static void init(void)
{
    dryoinit(LOG_FILE, FILE_NAME, PROJECT_NAME, DRYOX_XDG_STATE);
}

// dryolog version 0.1.0
int dryolog(Log_Level level, char *format, ...)
{
    // make a timestamp ts with current time since unix epoch
    char ts[DRYOX_TIMESTAMP_SIZE];
    time_t now = time(NULL);
    strftime(ts, sizeof(ts), "%m-%d-%Y %H:%M:%S", localtime(&now));

    // open file
    FILE *logfile = fopen(LOG_FILE, "a");
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

    // TODO: stderr suppression for ERROR and FATAL

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
        fprintf(logfile, "\n");
        vfprintf(stderr, format, args_for_error);
        fprintf(stderr, "\n");
        break;

    case LOG_FATAL:
        fprintf(logfile, "[%s] [FATAL] ", ts);
        vfprintf(logfile, format, args);
        fprintf(logfile, "\n");
        vfprintf(stderr, format, args_for_error);
        fprintf(stderr, "\n");
        break;
    }

    // end variadics
    va_end(args);
    va_end(args_for_error);

    // close file
    fclose(logfile);

    return 0;
}
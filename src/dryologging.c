#include "dryox/dryologging.h"
#include "dryox/dryoinit.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

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

/* this is defined in the header.
typedef enum
{
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  COUNTOF_LOG_LEVELS
} Log_Level;
*/

typedef enum
{
  TYPE_NORMAL,
  TYPE_STDERR,
} Type_Level;

static const char *LEVEL_NAMES[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL", "OUT_OF_LEVEL_RANGE_ERROR"};

typedef struct
{
  Log_Level level;
  Type_Level type;
} Type_Association;

Type_Association association_levels[COUNTOF_LOG_LEVELS] = {
    {LOG_DEBUG, TYPE_NORMAL}, {LOG_INFO, TYPE_NORMAL},  {LOG_WARN, TYPE_NORMAL},
    {LOG_ERROR, TYPE_STDERR}, {LOG_FATAL, TYPE_STDERR},
};

int dryolog_internal(Log_Level level, const char *file, int line, const char *func, char *format, ...)
{
  // dryoinit
  char *PATH = getenv("DRYOX_LITERAL_PATH");
  if (!PATH)
  {
    dryoinit(LOG_FILE, FILE_NAME, PROJECT_NAME, DRYOX_XDG_STATE);
  }
  else
  {
    dryoinit(LOG_FILE, FILE_NAME, PROJECT_NAME, DRYOX_LITERAL, PATH);
  }

  // getenvs
  char *is_quiet_buf = getenv("IS_QUIET");
  int is_quiet = is_quiet_buf != NULL && strcmp(is_quiet_buf, "0") != 0;

  // make a timestamp ts with current time since unix epoch
  char ts[DRYOX_TIMESTAMP_SIZE];
  time_t now = time(NULL);
  strftime(ts, sizeof(ts), "%m-%d-%Y %H:%M:%S", localtime(&now));

  // open file
  FILE *logfile = fopen(LOG_FILE, "a");
  if (logfile == NULL)
  {
    fprintf(stderr, "The libdryox module \"logging\" failed to open the log file.\n"
                    "FILE logfile may be undefined,\n"
                    "or logging.h has no access to it.\n"
                    "exiting logging as fatal.\n");
    return -1;
  }

  // parse level into Type_Association
  int type = association_levels[level].type;

  // start variadics
  va_list args;
  va_list args_for_error;
  va_start(args, format);
  va_copy(args_for_error, args);

  // TODO: stderr suppression for ERROR and FATAL

  // handle each level
  {
    fprintf(logfile, "%s: [%s] [%s] %s:%d:\n\t", file, ts, LEVEL_NAMES[level], func, line);
    vfprintf(logfile, format, args);
    fprintf(logfile, "\n---\n");
  }

  if (type == TYPE_STDERR && is_quiet == 0)
  {
    fprintf(stderr, "%s: %s:%d:\n\t", file, func, line);
    vfprintf(stderr, format, args_for_error);
    fprintf(stderr, "\n---\n");
  }

  // end variadics
  va_end(args);
  va_end(args_for_error);

  // close file
  fclose(logfile);

  return 0;
}
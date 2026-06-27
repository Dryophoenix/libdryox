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

TODO (refactor scope):
  - Only serve a guard for DRYOX_LITERAL_PATH instead of calling dryoinit each
    time. Currently dryoinit + fopen + fclose run on EVERY log call, which is
    wasteful filesystem work per line. Resolve the path + open the handle once.

  - Switch storage to a structured, lossless record format (logfmt-style):
        time=... level=ERROR file=<full path> line=24 func=... msg="..."
    This keeps every field (including the full absolute path) machine-parseable
    and greppable, and ends the "absolute path vs. clean output" tradeoff:
    storage stays rich, presentation decides what to show.

  - Treat human-readable prettiness as a VIEWER concern, not a storage concern.
    The viewer can render basename(file) (libgen.h, as dryoinit already uses for
    dirname), group/align fields, etc. Storage stays flat and chronological so
    grep/tail/awk keep working and every line is self-contained. Do NOT omit
    repeated fields or reorder entries in storage -- that breaks per-line
    self-containment and chronological order, the two properties a log relies on.

It serves the following extern functions:
  dryolog(Level level, char * message, ...);

It defines the following types:
  typedef enum Log_Level;
*/

#define DRYOX_TIMESTAMP_SIZE 32

static char PROJECT_NAME[] = "DryoX/libdryox";
static char FILE_NAME[] = "logging";
static char LOG_FILE[PATH_MAX];
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

int dryolog_internal(Log_Level level, const char *file, int line, const char *func, const char *format, ...)
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
    fprintf(logfile, "%s: [%s] [%s] %s:%d states:\n\t", file, ts, LEVEL_NAMES[level], func, line);
    vfprintf(logfile, format, args);
    fprintf(logfile, "\n\t---\n");
  }

  if (type == TYPE_STDERR && is_quiet == 0)
  {
    fprintf(stderr, "%s: %s:%d states:\n\t", file, func, line);
    vfprintf(stderr, format, args_for_error);
    fprintf(stderr, "\n\t---\n");
  }

  // end variadics
  va_end(args);
  va_end(args_for_error);

  // close file
  fclose(logfile);

  return 0;
}

int dryolog_external(Log_Level level, const char *file, int line, const char *func, char *msg)
{
  return dryolog_internal(level, file, line, func, msg);
}

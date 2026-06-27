#ifndef DRYOX_LOGGING_H
#define DRYOX_LOGGING_H

// Used for strerr in dryolog_errno.
#include <errno.h>
#include <string.h>
typedef enum
{
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  COUNTOF_LOG_LEVELS
} Log_Level;

int dryolog_internal(Log_Level level, const char *file, int line, const char *func, const char *format, ...);

int dryolog_external(Log_Level level, const char *file, int line, const char *func, char *msg);

#define dryolog(level, format, ...) dryolog_internal(level, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define dryolog_declare_errno(code, msg)                                                                               \
  do                                                                                                                   \
  {                                                                                                                    \
    dryolog(LOG_ERROR, "%s: %s", msg, strerror(code));                                                                 \
    errno = (code);                                                                                                    \
  } while (0);

#define dryolog_parrot_errno(errno, msg)                                                                               \
  do                                                                                                                   \
  {                                                                                                                    \
    dryolog(LOG_ERROR, "%s. Error %d: %s", msg, errno, strerror(errno));                                               \
  } while (0);

#endif
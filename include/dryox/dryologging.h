#ifndef DRYOX_LOGGING_H
#define DRYOX_LOGGING_H

typedef enum
{
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  COUNTOF_LOG_LEVELS
} Log_Level;

int dryolog_internal(Log_Level level, const char *file, int line, const char *func, char *format, ...);

#define dryolog(level, format, ...) dryolog_internal(level, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#endif
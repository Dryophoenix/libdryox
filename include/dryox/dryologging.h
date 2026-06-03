#ifndef DRYOX_LOGGING_H
#define DRYOX_LOGGING_H

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
} Log_Level;

extern int dryolog(Log_Level level, char *format, ...);

#endif
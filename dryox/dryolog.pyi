from enum import IntEnum

class LogLevel(IntEnum):
    DEBUG   = 0
    INFO    = 1
    WARN    = 2
    ERROR   = 3
    FATAL   = 4

def dryolog(level: LogLevel, msg: str) -> int | None: ...
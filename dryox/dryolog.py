from _dryox import ffi, lib
from enum import IntEnum
import inspect

class LogLevel(IntEnum):
    DEBUG   = 0
    INFO    = 1
    WARN    = 2
    ERROR   = 3
    FATAL   = 4


def dryolog(level: int, msg: str) -> int:
    frame = inspect.currentframe()
    if frame is not None:
        frame = frame.f_back
    line: int = frame.f_lineno
    if line is None:
        line = 0
    file: str = frame.f_code.co_filename
    if file is None:
        file = "errorNoFilename"
    func: str = frame.f_code.co_name
    if func is None:
        func = "errorNoFunctionName"
    lib.dryolog_external(level, file.encode(), line, func.encode(), msg.encode())
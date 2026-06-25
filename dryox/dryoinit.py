from _dryox import ffi, lib
# The warning here is benign, if you're using a linter.
# It just can't see the build artifact that is _dryox.
from enum import IntEnum
import inspect

class Dryox_XDG_Dir(IntEnum):
    DRYOX_XDG_UNSET = 0
    DRYOX_XDG_CONFIG = 1
    DRYOX_XDG_DATA = 2
    DRYOX_XDG_STATE = 3
    DRYOX_XDG_CACHE = 4
    DRYOX_LITERAL = 5

def dryoinit(filename: str, projectname: str, mode: Dryox_XDG_Dir) -> str:
    result = lib.dryoinit_external(filename.encode(), projectname.encode(), mode)
    if result == ffi.NULL:
        return None
    return ffi.string(result).decode()
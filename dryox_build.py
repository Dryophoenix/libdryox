import sys
from cffi import FFI

ffibuilder = FFI()

ffibuilder.cdef(
"""
    // dryologging.h

    typedef enum
    {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    COUNTOF_LOG_LEVELS
    } Log_Level;

    int dryolog_external(Log_Level level, const char *file, int line, const char *func, char *msg);
""")

ffibuilder.cdef(
"""
    // dryoinit.h

    typedef enum
    {
    DRYOX_XDG_UNSET,
    DRYOX_XDG_CONFIG,
    DRYOX_XDG_DATA,
    DRYOX_XDG_STATE,
    DRYOX_XDG_CACHE,
    DRYOX_LITERAL,
    } Dryox_XDG_Dir;

    char *dryoinit_external(char *filename, char *projectname, Dryox_XDG_Dir mode);
"""
)

ffibuilder.set_source("_dryox",
"""
    #include "dryox/dryoinit.h"
    #include "dryox/dryologging.h"
""",
    sources=['src/dryologging.c', 'src/dryoinit.c', 'src/utils/dryo_mkdirp.c'],
    include_dirs=['include', 'include/dryox'],
)

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)

import sys
from cffi import FFI

if not sys.platform.startswith('linux'):
    raise Exception("Linux support only this commit")

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

ffibuilder.set_source("_dryox",
"""
    #include "dryox/dryologging.h"
""",
    sources=['src/dryologging.c', 'src/dryoinit.c', 'src/utils/dryo_mkdirp.c'],
    include_dirs=['include', 'include/dryox'],
)

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)

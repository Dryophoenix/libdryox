# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

libdryox is the shared C library for the DryoX ecosystem (logging, XDG-style path/file
init, TOML config, and small utilities), exposed both as a native C library and as a
Python package (`dryox`) via cffi bindings. It is unstable; signatures that are already
public should stay stable, but the surface area is still growing.

## Build, Test, Install

C library (clang, Allman brace style, 120-col limit per `.clang-format`):
```shell
make libdryox        # builds libdryox.dylib (macOS) or .so (Linux)
make test             # builds libdryox, then builds+runs tests/testall
make clean            # removes build artifacts (tests/, *.o, the lib)
make cleanall         # clean + also removes libdryox.so/.dylib explicitly
make install          # PREFIX=/usr/local by default; installs lib + headers
make uninstall
```

Python bindings (cffi):
```shell
python dryox_build.py       # compiles the _dryox cffi extension directly
pip install -e .             # setup.py drives dryox_build.py's ffibuilder
```

Running a single test: there is no per-test make target. All tests are plain
`TEST_<module>()` C functions called in sequence from `src/tests/testall.c`'s `main()`.
To isolate one, either comment out the other `TEST_*()` calls in that file before
`make test`, or compile/run a single test file directly against the already-built lib, e.g.:
```shell
make libdryox
clang -I./include -I./vendor -I./include/dryox src/tests/dryoinit.c -o /tmp/t -L. -ldryox -Wl,-rpath,@loader_path/. && /tmp/t
```

## Architecture

- **`src/dryoinit.c`** is the foundational module. It resolves an XDG-style path
  (`DRYOX_XDG_CONFIG`/`DATA`/`STATE`/`CACHE`, or a literal override path via
  `DRYOX_LITERAL`), honoring `DENV_OVERRIDE_<X>` / `XDG_<X>_HOME` / `$HOME` env vars in
  that priority order, then creates the directory tree (via `dryo_mkdirp`) and touches
  the file.
- **`src/utils/dryo_mkdirp.c`** is `mkdir -p`, used by `dryoinit`.
- **`src/dryologging.c`** calls `dryoinit` on *every* `dryolog(...)` call to resolve the
  log file path, then opens/appends/closes it per call. The file's own header comment
  flags this as a known inefficiency slated for a future refactor (cache the resolved
  path/handle once instead of re-resolving per line).
- **`src/dryoconfig.c`** builds on `dryoinit` + vendored `tomlc17` to manage TOML config
  sections (declare a config file, declare/create a `[section]`). It is **not yet wired
  into the build**: it's absent from the makefile's `LIBDRYOX_SRC` and from
  `dryox_build.py`'s cffi `sources`, so it compiles standalone but doesn't ship in the
  built library or the Python bindings yet.
- **Dual C surface per module**: most modules expose a plain function plus an
  `_external` wrapper (`dryolog_internal`/`dryolog_external`,
  `dryoinit`/`dryoinit_external`). The `_external` variants are the ones declared in
  `dryox_build.py`'s `cdef(...)` blocks and are what the Python layer in `dryox/`
  (`dryox/dryoinit.py`, `dryox/dryolog.py`) actually calls through `_dryox`'s `ffi`/`lib`.
- **Tests** (`src/tests/*.c`) each define one `TEST_<module>(void)`, all invoked from
  `src/tests/testall.c`. Shared test helpers: `REPORT(result, msg)`
  (`include/dryox/tests/REPORT.h`) prints pass/fail, and
  `CACHESETENV`/`RESTOREENV` (`include/dryox/tests/ENVUTILS.h`) save/restore env vars
  around tests that exercise the `DENV_OVERRIDE_*`/`XDG_*_HOME`/`HOME` resolution order.

## Conventions

- Every module `.c` file under `src/` and `src/utils/` opens with a standard doc-comment
  block: `"<file>.c is a libdryox module."`, followed by `It serves the following extern
  functions:`, `It defines the following types:`, and (where relevant) a macros section.
  Check this block first when you need a module's public surface instead of grepping headers.

## Maintenance burdens / known gaps

- Those doc-comment blocks are hand-written prose, not generated from the headers — there
  is no script or check enforcing them. Whenever a function signature changes (param
  added/removed, renamed, new `_external` wrapper, etc.), the block must be updated by
  hand in the same change, or it silently drifts out of sync with the real code.
- `dryoconfig.c` is built/tested standalone but is not yet part of `LIBDRYOX_SRC` in the
  makefile or `dryox_build.py`'s `sources` — it ships in neither the compiled library nor
  the Python package yet.
- `dryologging.c`'s per-call `dryoinit()` + `fopen`/`fclose` is a known perf issue, not
  yet fixed (see the TODO in that file's header comment).
- The makefile's `install` target reserves `MANDIR` and has commented-out manpage install
  lines (`MAN:` prefix), but no manpages exist yet.

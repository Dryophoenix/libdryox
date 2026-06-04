# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Test Commands

```sh
make              # build libdryox.so (default)
make libdryox     # explicit alias for libdryox.so
make libdryoutils # build libdryoutils.so (standalone utility subset)
make test         # build and run all tests (outputs to tests/)
make clean        # remove all build artifacts and tests/ directory
```

Run a single test by compiling it directly — each test file exposes one `TEST_<module>()` function and can be compiled standalone against libdryox.so:
```sh
clang -I./include -I./vendor -I./include/dryox src/tests/dryoinit.c src/tests/testall.c \
  src/utils/dryo_mkdirp.c src/dryoinit.c src/dryologging.c -o tests/testall && ./tests/testall
```

## Architecture

**Two output libraries:**
- `libdryox.so` — main library: `src/dryologging.c` + `src/dryoinit.c` + utils
- `libdryoutils.so` — standalone utility subset for non-DryoX consumers (`src/utils/dryo_mkdirp.c` only)

**Module layout:**

| Module | Source | Header | Purpose |
|---|---|---|---|
| `dryoinit` | `src/dryoinit.c` | `include/dryox/dryoinit.h` | XDG path resolution + file init |
| `dryologging` | `src/dryologging.c` | `include/dryox/dryologging.h` | Structured file logging |
| `dryo_mkdirp` | `src/utils/dryo_mkdirp.c` | *(internal)* | `mkdir -p` in C |
| `dryo_tomlc17` | `src/utils/dryo_tomlc17.c` | `include/dryox/utils/dryo_tomlc17.h` | Stable wrapper over vendored `tomlc17` |

**`dryoinit` path resolution** — `DENV_OVERRIDE_<X>` > `XDG_<X>_HOME` > `~/.{config,local/share,local/state,.cache}/<project>/<file>`. `DRYOX_LITERAL` bypasses XDG entirely; a path must be passed as a variadic argument. `DRYOX_XDG_UNSET` is illegal and returns `-1`.

**`dryologging`** — uses a `__attribute__((constructor))` to call `dryoinit` at load time, writing the log path into the static `LOG_FILE`. `LOG_ERROR` and `LOG_FATAL` mirror to stderr in addition to the log file.

**Test pattern** — each module has `src/tests/<module>.c` exposing `TEST_<module>()`. `src/tests/testall.c` includes all headers and calls them in sequence. Tests create artifacts inside `tests/` (gitignored).

## Conventions

- **Compiler:** clang; flags: `-Wall -Wextra -fPIC`
- **Style:** Allman braces, 120 column limit (`.clang-format`)
- **Function prefixes:** `dryox_` for public API, `dryou_` for utilities (libdryoutils), `dryo_` for internal utils
- **Include paths:** `./include`, `./vendor`, `./include/dryox` — use `#include "dryox/module.h"` for public headers, `#include "utils/module.h"` for internal utils
- **New modules:** use `templates.c` / `templates.h` as the starting template; add source to `_LIBDRYOX_SRC` in the makefile and add a test entry to `testall.c`
- **Error returns:** functions return `0` on success, `-1` on failure; fatal conditions print to `stderr` with `__FILE__`, `__LINE__`, `__func__`

# libdryox

Libdryox is a basic library for the greater DryoX ecosystem. It contains anywhere
from logging functionality to TOML parsing; essentially anything that any two
child projects do or will need.

## Stability

libdryox is unstable and is due to change frequently, although established signatures
should remain stable for the short forseeable future.

## Utilities and Commands

Libdryox provides a number of utilities, and (TODO:) each C utility has
a manpage in section 3.

Most of these utilities have further functionality that is not given justice
by these short descriptions, so especially using them in C, I recommend looking
at the manpages.

Regardless, here's short usages for each:

### Dryoinit (C, Python)

In C:
```c
int dryoinit(char *file_out, char *filename, char *projectname, Dryox_XDG_Dir mode, ...);

char my_filepath_buffer[PATH_MAX];
dryoinit(my_filepath_buffer, "my_module.conf", "my_project", DRYOX_XDG_CONFIG)
// or ..._DATA, or ..._STATE, or ..._CACHE

// dryoinit then seeks and creates the directories, and touches the file.

printf("%s\n", my_filepath_buffer);

// Expected: /home/my_user/.config/my_project/my_module.conf
```

In Python:
```python
def dryoinit(filename: str, projectname: str, mode: Dryox_XDG_Dir) -> str

my_filepath = dryoinit("my_module.conf", "my_project", DRYOX_XDG_CONFIG)
# or ..._DATA, or ..._STATE, or ..._CACHE

# dryoinit then seeks and creates the directories, then touches the file.

print(my_filepath)

# Expected: /home/my_user/.config/my_project/my_module.conf
```

### Dryolog (C, Python)

In C:
```c
#define dryolog(level, format, ...)

dryolog(LOG_DEBUG, "%s\n", "Hello from dryologging!");
// or ..._INFO or ..._WARN or ..._ERROR or ..._FATAL

// dryolog checks to see if the logging folder exists in XDG_CACHE_HOME, and
// tries to make it according to dryoinit's spec, loudly failing if it can't.

// on "sh cat path/to/my_config_file",
/* Expected:
 *
 * path/to/my/program.c: [mm-dd-yyyy hh:mm:ss] [DEBUG] function:line states:
 *	Hello from dryologging!
 *	---
 *
 */
```

In Python:
```python
def dryolog(level: int, msg: str) -> int

dryolog(LOG_DEBUG, "Hello from dryologging!")
# or ..._INFO or ..._WARN or ..._ERROR or ..._FATAL

# dryolog checks to see if the logging folder exists in XDG_CACHE_HOME, and
# tries to make it according to dryoinit's spec, loudly failing if it can't.

# on "sh cat path/to/my_config_file",
# Expected:
#
# path/to/my/program.c: [mm-dd-yyyy hh:mm:ss] [DEBUG] function:line states:
#  	Hello from dryologging!
#   ---
```


## Dependencies

* tomlc17: libdryox is currently using version 260618.

## License

Apache 2.0, see LICENSE
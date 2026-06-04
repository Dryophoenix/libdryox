CC                	= clang
CFLAGS				= -I./include -I./vendor -I./include/dryox -Wall -Wextra -fPIC
LDFLAGS				= -shared

all					: libdryox.so

# -- all tests --
_TESTING_SRC 		= dryo_mkdirp.c dryoinit.c dryologging.c
TESTING_SRC 		= $(addprefix src/tests/, $(_TESTING_SRC))
# --

# -- libdryoutils --
_LIBDRYOUTILS_SRC 	= dryo_mkdirp.c
LIBDRYOUTILS_SRC	= $(addprefix src/utils/, $(_LIBDRYOUTILS_SRC))
LIBDRYOUTILS_OBJ 	= $(LIBDRYOUTILS_SRC:.c=.o)

# an edge case for tooling partially associated with dryoX
libdryoutils.so : $(LIBDRYOUTILS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBDRYOUTILS_OBJ)
# --

# -- libdryox (main) --
_LIBDRYOX_SRC 		= dryologging.c dryoinit.c
LIBDRYOX_SRC		= $(addprefix src/, $(_LIBDRYOX_SRC))
LIBDRYOX_OBJ     	= $(LIBDRYOX_SRC:.c=.o)

libdryox.so : $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ)
# --

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY 				: all libdryox libdryoutils clean test

libdryox			: libdryox.so
libdryoutils		: libdryoutils.so

test 				: libdryox.so
	mkdir -p -- tests
	$(CC) $(CFLAGS) $(TESTING_SRC) src/tests/testall.c -o tests/testall \
	-L. -ldryox -Wl,-rpath,'$$ORIGIN/..'
	./tests/testall

clean 				:
	rm -rf -- tests
	rm -f -- $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ) libdryox.so libdryoutils.so


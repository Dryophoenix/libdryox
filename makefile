VERSION				:= $(shell cat VERSION)
CC                	= clang
CFLAGS				= -I./include -I./vendor -I./include/dryox -Wall -Wextra -fPIC
UNAME 				:= $(shell uname)

ifeq ($(UNAME), Darwin)
	LIB_EXT 		= dylib
	LDFLAGS 		= -dynamiclib
	RPATH			= -Wl -rpath @loader_path/..
else
	LIB_EXT 		= so
	LDFLAGS			= -shared
	RPATH			= -Wl -rpath '$$ORIGIN/..'
endif

LIBDRYOX			= libdryox.$(LIB_EXT)

all					: $(LIBDRYOX)

# -- all tests --
_TESTING_SRC 		= dryo_mkdirp.c dryoinit.c dryologging.c
TESTING_SRC 		= $(addprefix src/tests/, $(_TESTING_SRC))
# --

# -- libdryox (main, all) --
_UTILS_SRC			= dryo_mkdirp.c
_LIBDRYOX_SRC 		= dryologging.c dryoinit.c
LIBDRYOX_SRC		= $(addprefix src/, $(_LIBDRYOX_SRC)) $(addprefix src/utils/, $(_UTILS_SRC))
LIBDRYOX_OBJ     	= $(LIBDRYOX_SRC:.c=.o)

$(LIBDRYOX) 		: $(LIBDRYOX_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBDRYOX_OBJ)
# --

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY 				: all libdryox clean test install uninstall

libdryox			: $(LIBDRYOX)

test 				: all
	mkdir -p -- tests
	$(CC) $(CFLAGS) $(TESTING_SRC) src/tests/testall.c -o tests/testall \
	-L. -ldryox $(RPATH)
	./tests/testall

clean 				:
	rm -rf -- tests
	rm -f -- $(LIBDRYOX_OBJ) $(LIBDRYOX)

cleanall			:
	rm -rf -- tests
	rm -f -- $(LIBDRYOX_OBJ) $(LIBDRYOX)
	rm -f -- libdryox.so libdryox.dylib

PREFIX				?= /usr/local
INCLUDEDIR			?= $(PREFIX)/include
LIBDIR				?= $(PREFIX)/lib
MANDIR				?= $(PREFIX)/share/man/man3
# MAN: Although man pages don't exist yet, mandir is to remain set.

INSTALL				= install
INSTALL_PROGRAM		= $(INSTALL) -m 755
INSTALL_DATA		= $(INSTALL) -m 644

install				: all
	$(INSTALL) -d $(DESTDIR)$(LIBDIR)
	$(INSTALL) -d $(DESTDIR)$(INCLUDEDIR)/dryox
#	MAN: $(INSTALL) -d $(DESTDIR)$(MANDIR)
#	Above call is unused because no manpages exist yet.
	$(INSTALL_PROGRAM) $(LIBDRYOX) $(DESTDIR)$(LIBDIR)/$(LIBDRYOX)
	$(INSTALL_DATA) include/dryox/*.h $(DESTDIR)$(INCLUDEDIR)/dryox/
# 	MAN: $(INSTALL_DATA) libdryox.3 $(DESTDIR)$(MANDIR)/libdryox.3
#	Above call is unused because no manpages exist yet.
	[ -z "$(DESTDIR)" ] && [ "$$(uname)" = "Linux" ] ldconfig $(LIBDIR) || true

uninstall			:
	rm -f  	$(DESTDIR)$(LIBDIR)/$(LIBDRYOX)
	rm -rf 	$(DESTDIR)$(INCLUDEDIR)/dryox
#	MAN: rm -f  	$(DESTDIR)$(MANDIR)/libdryox.1
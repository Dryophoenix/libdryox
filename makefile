CC                	= clang
CFLAGS				= -I./include -I./vendor -I./include/dryox -Wall -Wextra -fPIC
UNAME 				:= $(shell uname)

ifeq ($(UNAME), Darwin)
	LIB_EXT 		= dylib
	LDFLAGS 		= -dynamiclib
	RPATH			= -Wl -rpath, @loader_path/..
else
	LIB_EXT 		= so
	LDFLAGS			= -shared
	RPATH			= -Wl -rpath, '$$ORIGIN/..'
endif

LIBDRYOX			= libdryox.$(LIB_EXT)
LIBDRYOUTILS		= libdroyutils.$(LIB_EXT)

all					: $(LIBDRYOX) $(LIBDRYOUTILS)

# -- all tests --
_TESTING_SRC 		= dryo_mkdirp.c dryoinit.c dryologging.c
TESTING_SRC 		= $(addprefix src/tests/, $(_TESTING_SRC))
# --

# -- libdryoutils --
_LIBDRYOUTILS_SRC 	= dryo_mkdirp.c
LIBDRYOUTILS_SRC	= $(addprefix src/utils/, $(_LIBDRYOUTILS_SRC))
LIBDRYOUTILS_OBJ 	= $(LIBDRYOUTILS_SRC:.c=.o)

# an edge case for tooling partially associated with dryoX
$(LIBDRYOUTILS) 	: $(LIBDRYOUTILS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBDRYOUTILS_OBJ)
# --

# -- libdryox (main) --
_LIBDRYOX_SRC 		= dryologging.c dryoinit.c
LIBDRYOX_SRC		= $(addprefix src/, $(_LIBDRYOX_SRC))
LIBDRYOX_OBJ     	= $(LIBDRYOX_SRC:.c=.o)

$(LIBDRYOX) 		: $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ)
# --

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY 				: all libdryox libdryoutils clean test install uninstall

libdryox			: $(LIBDRYOX)
libdryoutils		: $(LIBDRYOUTILS)

test 				: all
	mkdir -p -- tests
	$(CC) $(CFLAGS) $(TESTING_SRC) src/tests/testall.c -o tests/testall \
	-L. -ldryox $(RPATH)
	./tests/testall

clean 				:
	rm -rf -- tests
	rm -f -- $(LIBDRYOX_OBJ) $(LIBDRYOUTILS_OBJ) $(LIBDRYOX) $(LIBDRYOUTILS)

PREFIX				?= /usr/local
INCLUDEDIR			?= $(PREFIX)/include
LIBDIR				?= $(PREFIX)/lib
MANDIR				?= $(PREFIX)/share/man/man3

INSTALL				= install
INSTALL_PROGRAM		= $(INSTALL) -m 755
INSTALL_DATA		= $(INSTALL) -m 644



install				: all
	$(INSTALL) -d $(DESTDIR)$(LIBDIR)
	$(INSTALL) -d $(DESTDIR)$(INCLUDEDIR)/dryox
	# $(INSTALL) -d $(DESTDIR)$(MANDIR)
	$(INSTALL_PROGRAM) $(LIBDRYOX) $(DESTDIR)$(LIBDIR)/$(LIBDRYOX)
	$(INSTALL_PROGRAM) $(LIBDRYOUTILS) $(DESTDIR)$(LIBDIR)/$(LIBDRYOUTILS)
	$(INSTALL_DATA) include/dryox/*.h $(DESTDIR)$(INCLUDEDIR)/dryox/
	# $(INSTALL_DATA) libdryox.1 $(DESTDIR)$(MANDIR)/libdryox.1
	# $(INSTALL_DATA) libdryoutils.1 $(DESTDIR)$(MANDIR)/libdryoutils.1
	[ -z "$(DESTDIR)" ] && [ "$$(uname)" = "Linux" ] ldconfig $(LIBDIR) || true

uninstall			:
	rm -f  	$(DESTDIR)$(LIBDIR)/$(LIBDRYOX)
	rm -f  	$(DESTDIR)$(LIBDIR)/$(LIBDRYOUTILS)
	rm -rf 	$(DESTDIR)$(INCLUDEDIR)/dryox
	rm -f  	$(DESTDIR)$(MANDIR)/libdryox.1
	rm -f	$(DESTDIR)$(MANDIR)/libdryoutils.1
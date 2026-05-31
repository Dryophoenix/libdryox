CC = gcc
CFLAGS = -I./include -Wall -Wextra -fPIC
LDFLAGS = -shared

ODIR = obj
TARGET = libdryox.so

_SRC = logging.c utils.c init.c
SRC = $(patsubst %,src/%,$(_SRC))
OBJ = $(patsubst src/%.c,$(ODIR)/%.o,$(SRC))

_DEPS = dryox/logging.h dryox/utils.h dryox/init.h
DEPS = $(patsubst %,include/%,$(_DEPS))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

all: $(TARGET)

test_mkdirp: src/test.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: all test clean

test: test_mkdirp
	./test_mkdirp

clean:
	rm -f $(ODIR)/*.o $(TARGET) test_mkdirp
	rm -rf tests
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: main.c libppm.a
	$(CC) $(CFLAGS) -o main main.c -L. -lppm

libppm.a: renderer.h renderer.c
	$(CC) -c $(CFLAGS) renderer.c; \
	ar -rcs libppm.a *.o;

.PHONY: all


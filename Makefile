CC = clang
CFLAGS= -Wall -Oz
LDFLAGS =


CSRC= rmdir.c ls.c 
UTILS = $(CSRC:%.c=bin/%)

.PHONY: all clean



all: $(UTILS)

bin:
	mkdir -p $@


bin/%:%.c | bin
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf bin

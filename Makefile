CC = clang
CFLAGS= -Wall -Oz
LDFLAGS =


UTILS = ls

.PHONY: all clean



all: bin bin/$(UTILS)

bin:
	mkdir -p $@


bin/%:%.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf bin

.PHONY: $(UTILS)

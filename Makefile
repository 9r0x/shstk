CC=gcc
#-Wall -O3 -ggdb3 -march=native -mtune 
CFLAGS=-mshstk -g
LDFLAGS=-Wl,-z,shstk

ifeq ($(debug), 1)
	CFLAGS += -DDEBUG
endif

SRC = $(wildcard *.c)
BIN = $(patsubst %.c,bin/%,$(SRC))

all: bin $(BIN)

test_shstk: $(BIN)
	@for i in $^; do echo $$i && readelf -n $$i | grep SHSTK; done

bin/%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

bin:
	mkdir -p bin

clean:
	rm -f bin/*
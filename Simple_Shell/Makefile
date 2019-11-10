CC := gcc

LIB := $(shell find lib -type f -name *.o)
SRC := $(shell find src -not -path '*/\.*' -type f -name *.c)
INC := -I include

DFLAGS := -g -DDEBUG -DCOLOR
CFLAGS := $(INC) -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast 


.PHONY: clean all setup

all: setup
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o bin/53shell

debug: setup
	$(CC) $(CFLAGS) $(DFLAGS) $(LIB) $(SRC) -o bin/53shell

setup:
	mkdir -p bin

clean:
	$(RM) -r bin

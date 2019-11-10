CC = gcc
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
DFLAGS := -g -DDEBUG 
CFLAGS = -Wall -Werror -I$(INC_DIR) $(DFLAGS)
TEST_SRCS := $(wildcard tests/*.c)
TESTS := $(patsubst %.c,%,$(TEST_SRCS))

LDFLAGS := ../build/icsmm.o ../build/helpers.o ../lib/icsutil.o
EFLAGS := -Wall -Werror $(DFLAGS) -I../$(INC_DIR)
PRG_SUFFIX=.out

export LDFLAGS
export EFLAGS
export PRG_SUFFIX

all: setup icsmm.o helpers.o 
	$(MAKE) -C tests
	mv tests/*$(PRG_SUFFIX) bin/

setup:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)

icsmm.o: $(SRC_DIR)/icsmm.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/icsmm.c -o $(BUILD_DIR)/$@

helpers.o: $(SRC_DIR)/helpers.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/helpers.c -o $(BUILD_DIR)/$@

clean:
	rm -rf bin/ build/

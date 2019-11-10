CC = gcc
BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
CFLAGS = -Wall -Werror -I$(INC_DIR) 
DFLAGS := -g -DDEBUG
SRCS := $(wildcard $(SRC_DIR)/*.c)

LDFLAGS := -lpthread -lncurses 

all: setup
	$(CC) $(SRCS) $(CFLAGS) -o $(BIN_DIR)/hw6 $(LDFLAGS)

setup:
	@mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

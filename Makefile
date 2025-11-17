CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -O2 -Iinclude
BIN_DIR = bin

SOURCES = $(wildcard src/*.c)
TARGETS = $(patsubst src/%.c,$(BIN_DIR)/%,$(SOURCES))

all: $(TARGETS)

$(BIN_DIR)/%: src/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all clean

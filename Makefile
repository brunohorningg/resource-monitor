
CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -O2 -Iinclude
BIN_DIR = bin
TEST_BIN_DIR = $(BIN_DIR)/tests

PROFILER_SRC = src/main.c src/cpu_monitor.c src/memory_monitor.c src/io_monitor.c
NAMESPACE_SRC = src/namespace_analyzer.c
CGROUP_SRC = src/cgroup_manager.c

all: $(BIN_DIR)/resource_profiler $(BIN_DIR)/namespace_analyzer $(BIN_DIR)/cgroup_manager tests

$(BIN_DIR)/resource_profiler: $(PROFILER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(PROFILER_SRC) -o $@

$(BIN_DIR)/namespace_analyzer: $(NAMESPACE_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(NAMESPACE_SRC) -o $@

$(BIN_DIR)/cgroup_manager: $(CGROUP_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CGROUP_SRC) -o $@

tests: $(TEST_BIN_DIR)/test_cpu $(TEST_BIN_DIR)/test_memory $(TEST_BIN_DIR)/test_io

$(TEST_BIN_DIR)/test_cpu: tests/test_cpu.c src/cpu_monitor.c | $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) tests/test_cpu.c src/cpu_monitor.c -o $@

$(TEST_BIN_DIR)/test_memory: tests/test_memory.c src/memory_monitor.c | $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) tests/test_memory.c src/memory_monitor.c -o $@

$(TEST_BIN_DIR)/test_io: tests/test_io.c src/io_monitor.c | $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) tests/test_io.c src/io_monitor.c -o $@

$(BIN_DIR) $(TEST_BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all clean tests

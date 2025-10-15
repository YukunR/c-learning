# Makefile for hashtable implementations testing

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O0
INCLUDES = -I./hashtable \
           -I../common \
           -I../dynamic_array \
           -I../doubly_circular_list \
           

# Source files - Common dependencies
COMMON_SRCS = ../common/common.c
DYNAMIC_ARRAY_SRCS = ../dynamic_array/dynamic_array.c
LIST_SRCS = ../doubly_circular_list/doubly_circular_list.c
HASHTABLE_COMMON_SRCS = ./hashtable.c \
                        ./hashtable_internal.c
HASH_SRCS = ./hash.c

# Implementation-specific sources
CHAINING_SRCS = ./hashtable_chaining.c
OA_SRCS = ./hashtable_oa.c

# Object files
COMMON_OBJS = $(COMMON_SRCS:.c=.o) $(DYNAMIC_ARRAY_SRCS:.c=.o) $(LIST_SRCS:.c=.o) $(HASHTABLE_COMMON_SRCS:.c=.o)
HASH_OBJS = $(HASH_SRCS:.c=.o)
CHAINING_OBJS = $(CHAINING_SRCS:.c=.o)
OA_OBJS = $(OA_SRCS:.c=.o)

# Test executables
TEST_CHAINING = test_chaining
TEST_OA = test_oa

# Default target - build all tests
all: $(TEST_CHAINING) $(TEST_OA)

# Build chaining hashtable test
$(TEST_CHAINING): $(COMMON_OBJS) $(HASH_OBJS) $(CHAINING_OBJS) ./test_chaining.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ ./test_chaining.c $(COMMON_OBJS) $(HASH_OBJS) $(CHAINING_OBJS)

# Build open addressing hashtable test
$(TEST_OA): $(COMMON_OBJS) $(HASH_OBJS) $(OA_OBJS) ./test_oa.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ ./test_oa.c $(COMMON_OBJS) $(HASH_OBJS) $(OA_OBJS)

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run tests
test-chaining: $(TEST_CHAINING)
	@echo "Running chaining hashtable tests..."
	./$(TEST_CHAINING)

test-oa: $(TEST_OA)
	@echo "Running open addressing hashtable tests..."
	./$(TEST_OA)

# Run all available tests
test: test-chaining test-oa
	@echo "All available tests completed!"

# Performance testing
perf-chaining: $(TEST_CHAINING)
	@echo "Running chaining hashtable performance tests..."
	./$(TEST_CHAINING) --performance

perf-oa: $(TEST_OA)
	@echo "Running open addressing hashtable performance tests..."
	./$(TEST_OA) --performance

# Memory leak detection
valgrind-chaining: $(TEST_CHAINING)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_CHAINING)

valgrind-oa: $(TEST_OA)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_OA)

valgrind: valgrind-chaining valgrind-oa
	@echo "Memory leak detection completed!"

# Clean build artifacts
clean:
	rm -f $(COMMON_OBJS) $(HASH_OBJS) $(CHAINING_OBJS) $(OA_OBJS) $(TEST_CHAINING) $(TEST_OA)

# Clean and rebuild
rebuild: clean all

# Create test directory structure (if needed)
setup:
	@mkdir -p tests
	@echo "Test directory structure created"

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build all test executables"
	@echo "  test-chaining    - Run chaining hashtable tests"
	@echo "  test-oa          - Run open addressing hashtable tests"
	@echo "  test             - Run all available tests"
	@echo "  perf-chaining    - Run chaining performance tests"
	@echo "  perf-oa          - Run open addressing performance tests"
	@echo "  valgrind-chaining- Run chaining tests with valgrind"
	@echo "  valgrind-oa      - Run open addressing tests with valgrind"
	@echo "  valgrind         - Run all tests with valgrind"
	@echo "  clean            - Remove build artifacts"
	@echo "  rebuild          - Clean and rebuild"
	@echo "  setup            - Create test directory structure"
	@echo "  help             - Show this help message"

.PHONY: all test test-chaining test-oa valgrind valgrind-chaining valgrind-oa perf-chaining perf-oa clean rebuild setup help
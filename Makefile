
CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror -I -Iinclude -ggdb -fsanitize=address -fsanitize=undefined

DEBUG=-DDEBUG

# Directories
BUILD_DIR = build
SRC_DIR = src

# Targets
TARGET = main

# Object files for each relevant source file
OBJS = $(BUILD_DIR)/main.o


# Ensure directories exist before building
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "\n"
	@echo "Build successful! Executable: $@"


# For each C++ file we write, add a rule to compile it like the below list of chunks
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -rf $(BUILD_DIR) *.ppm *.txt main

.PHONY: all clean
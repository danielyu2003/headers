# Compiler and C standard
CC := gcc
STD := c2x

# Directories
SRC_DIR := examples
BUILD_DIR := builds
HEADER_DIR := src

# Flags
CFLAGS := -Wall -Wextra -Werror -I$(HEADER_DIR) -std=$(STD)

# Example source files
EXAMPLES := $(wildcard $(SRC_DIR)/*.c)

# Corresponding executables (place them in the build directory)
EXECUTABLES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(EXAMPLES))

# Default target
all: $(BUILD_DIR) $(EXECUTABLES)

# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile each example into an executable
$(BUILD_DIR)/%: $(SRC_DIR)/%.c $(wildcard $(HEADER_DIR)/*.h)
	$(CC) $(CFLAGS) -o $@ $<

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Display a message with available commands
help:
	@echo "Available commands:"
	@echo "  make          - Build all example programs"
	@echo "  make clean    - Remove all compiled files"
	@echo "  make help     - Show this help message"

.PHONY: all clean help
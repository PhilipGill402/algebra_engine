# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# Directories
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = build
TARGET = test # name of your final executable

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILE := $(TEST_DIR)/test.c
ALL_SRC := $(SRC_FILES) $(TEST_FILE)

# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJ_FILES += $(OBJ_DIR)/test.o

# Default target
all: $(TARGET)

# Link all object files into final program
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

# Compile src/*.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test/test.c
$(OBJ_DIR)/test.o: $(TEST_FILE) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build folder exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean

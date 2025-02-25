# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -ansi -pedantic -g

# Executable name
EXECUTABLE = assembler

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header files
DEPS = $(wildcard $(INCLUDE_DIR)/*.h)

# Default target
all: $(EXECUTABLE)

# Rule to create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Rule to build the executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

# Run rule
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run

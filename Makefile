# Compiler
CC := c++

# Compiler flags
CCFLAGS := -std=c++20 -Wall -Wextra -pedantic -pthread

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable name
TARGET := $(BUILD_DIR)/program

# Default target
all: $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $^ -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all run clean

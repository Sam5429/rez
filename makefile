# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Directories
SRC_DIR = src
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/dataStruct/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXECUTABLE = rez

# Default target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/dataStruct/%.o: $(SRC_DIR)/dataStruct/%.cpp | $(BUILD_DIR)/dataStruct
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/dataStruct:
	mkdir -p $(BUILD_DIR)/dataStruct

# Phony targets
.PHONY: all clean

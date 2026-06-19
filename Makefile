CXX      := g++
TARGET   := cal

# Source files
SRC      := cal.cc

# Base compiler flags (Warnings)
CXXFLAGS := -Wall -Wextra -std=c++11

# Mode-specific flags
DEBUG_FLAGS := -fsanitize=address -g -O0 -DDEBUG
PROD_FLAGS  := -O3 -DNDEBUG

# Default target (defaults to prod, change to debug if you prefer)
.PHONY: all
all: prod

# Debug build
.PHONY: debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Production build
.PHONY: prod
prod: CXXFLAGS += $(PROD_FLAGS)
prod: $(TARGET)

# Link rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET)
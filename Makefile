CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS =

# Directories
SRCDIR = src
HEADERDIR = src/headers
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Target executable
TARGET = $(BINDIR)/pacman.exe

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Run the game
run: $(TARGET)
	./$(TARGET)

# Install dependencies (for Windows)
install-deps:
	# This is a placeholder for Windows-specific dependencies
	# The winmm library should be available by default on Windows
	echo "Dependencies should be available on Windows by default"

# Help
help:
	@echo "Available targets:"
	@echo "  all        - Build the game"
	@echo "  clean      - Remove build files"
	@echo "  run        - Build and run the game"
	@echo "  install-deps - Install dependencies (Windows only)"

.PHONY: all clean run install-deps help

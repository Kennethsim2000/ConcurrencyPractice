# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g -pthread

# Source files and their corresponding targets
SRC = orderBook.cpp produceConsume.cpp readWrite.cpp concurrentHashmap.cpp
TARGETS = orderBook produceConsume readWrite concurrentHashmap

# Default target: Build all programs
all: $(TARGETS)

# Define how to build each program
orderBook: orderBook.cpp
	$(CXX) $(CXXFLAGS) -o orderBook orderBook.cpp
	chmod +x orderBook

produceConsume: produceConsume.cpp
	$(CXX) $(CXXFLAGS) -o produceConsume produceConsume.cpp
	chmod +x produceConsume

readWrite: readWrite.cpp
	$(CXX) $(CXXFLAGS) -o readWrite readWrite.cpp
	chmod +x readWrite

concurrentHashmap: concurrentHashmap.cpp
	$(CXX) $(CXXFLAGS) -o concurrentHashmap concurrentHashmap.cpp
	chmod +x concurrentHashmap

# Clean the build artifacts
clean:
	rm -f $(TARGETS)

# Test target
check: all
	# Add your test commands here, for example:
	./orderBook
	./produceConsume
	./readWrite
	./concurrentHashmap

# Distribution check (commonly used in autotools projects)
distcheck: check
	@echo "Distribution check passed"

# Phony targets
.PHONY: all clean check distcheck
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

produceConsume: produceConsume.cpp
	$(CXX) $(CXXFLAGS) -o produceConsume produceConsume.cpp

readWrite: readWrite.cpp
	$(CXX) $(CXXFLAGS) -o readWrite readWrite.cpp

concurrentHashmap: concurrentHashmap.cpp
	$(CXX) $(CXXFLAGS) -o concurrentHashmap concurrentHashmap.cpp
	
# Clean the build artifacts
clean:
	rm -f $(TARGETS)

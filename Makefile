# Compiler
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -pthread 
#-wall and -Wextra is used to enable warning messages
#-g is to include debug information
#-pthread is used to enable multi-threading support

# Source files and their corresponding targets
SRC = orderBook.cpp produceConsume.cpp readWrite.cpp concurrentHashmap.cpp 
TARGETS = orderBook produceConsume readWrite concurrentHashmap #Corresponding executable

# Default rule when you run make: Build all programs
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

# make clean: Deletes all the compiled binaries
clean:
	rm -f $(TARGETS)

# make check: Builds everything, then run the executables
check: all
	# Add your test commands here, for example:
	./orderBook
	./produceConsume
	./readWrite
	./concurrentHashmap

# make distcheck: Distribution check, runs check, then echo
distcheck: check
	@echo "Distribution check passed"

# Declares that all, clean, check and distcheck are not files
.PHONY: all clean check distcheck
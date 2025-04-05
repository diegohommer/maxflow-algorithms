## Directories
IDIR = include
SDIR = src
ODIR = obj
BDIR = bin

## Compiler and flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -O2 -fopenmp -I$(IDIR)
CFLAGS = -O2 -I$(IDIR)

## Source files
MAIN_SRC = $(SDIR)/main.cpp $(SDIR)/algorithms/graph.cpp $(SDIR)/algorithms/ford_fulk.cpp $(SDIR)/algorithms/path_finding.cpp
GEN_SRC = $(SDIR)/generator/new_washington.c

## Object files
MAIN_OBJ = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(MAIN_SRC))
GEN_OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(GEN_SRC))

## Executables
MAIN_EXEC = $(BDIR)/run_ford
GEN_EXEC = $(BDIR)/gengraph

## Rules
all: $(MAIN_EXEC) $(GEN_EXEC)

$(MAIN_EXEC): $(MAIN_OBJ)
	@mkdir -p $(BDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GEN_EXEC): $(GEN_OBJ)
	@mkdir -p $(BDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)  
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ODIR) $(BDIR)

.PHONY: all clean
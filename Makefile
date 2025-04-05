# ========== Directory Structure ==========
IDIR := include
SDIR := src
ODIR := obj
BDIR := bin

# ========== Compiler and Flags ==========
CXX := g++
CC := gcc
CXXFLAGS := -std=c++17 -O2 -fopenmp -I$(IDIR)
CFLAGS := -O2 -I$(IDIR)
LDFLAGS := -fopenmp  # Added for OpenMP linking

# ========== Source Files ==========
ALG_SRCS := $(wildcard $(SDIR)/algorithms/*.cpp)
DS_SRCS := $(wildcard $(SDIR)/data_structs/*.cpp)

MAIN_SRCS := $(SDIR)/main.cpp $(ALG_SRCS) $(DS_SRCS)
BOOST_SRCS := $(SDIR)/boost_maxflow.cpp
GEN_SRCS := $(SDIR)/generator/new_washington.c

# ========== Object Files ==========
MAIN_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(MAIN_SRCS))
BOOST_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(BOOST_SRCS))
GEN_OBJS := $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(GEN_SRCS))

# ========== Executables ==========
EXEC_MAIN := $(BDIR)/flow_solver
EXEC_BOOST := $(BDIR)/flow_boost
EXEC_GEN := $(BDIR)/graph_generator

# ========== Build Rules ==========
all: $(EXEC_MAIN) $(EXEC_BOOST) $(EXEC_GEN)

$(EXEC_MAIN): $(MAIN_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_BOOST): $(BOOST_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_GEN): $(GEN_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ODIR) $(BDIR)

.PHONY: all clean
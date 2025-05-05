# ========== Directory Structure ==========
IDIR := include
SDIR := src
ODIR := obj
BDIR := bin

# ========== Compiler and Flags ==========
CXX := g++
CC := gcc
CXXFLAGS := -std=c++17 -O3 -fopenmp -I$(IDIR)
CFLAGS := -O3 -I$(IDIR) -w
LDFLAGS := -fopenmp  # Added for OpenMP linking

# ========== Source Files ==========
ALG_SRCS := $(wildcard $(SDIR)/algorithms/*.cpp)
DS_SRCS := $(wildcard $(SDIR)/data_structs/*.cpp)
HELP_SRCS := $(wildcard $(SDIR)/helper/*.cpp)

MAIN_SRCS := $(SDIR)/main.cpp $(HELP_SRCS) $(ALG_SRCS) $(DS_SRCS)
BOOST_SRCS := $(SDIR)/boost_maxflow.cpp
GEN_SRCS := $(SDIR)/generator/new_washington.c

# Tournament files
TOUR_GEN_SRCS := $(SDIR)/tournament/generator/generate_tournament.cpp
TOUR_SOLVER_SRCS := $(SDIR)/tournament/main.cpp $(ALG_SRCS) $(DS_SRCS)

# ========== Object Files ==========
MAIN_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(MAIN_SRCS))
BOOST_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(BOOST_SRCS))
GEN_OBJS := $(patsubst $(SDIR)/%.c,$(ODIR)/%.o,$(GEN_SRCS))

# Tournament Object Files
TOUR_GEN_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(TOUR_GEN_SRCS))
TOUR_SOLVER_OBJS := $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(TOUR_SOLVER_SRCS))

# ========== Executables ==========
EXEC_MAIN := $(BDIR)/flow_solver
EXEC_BOOST := $(BDIR)/flow_boost
EXEC_GEN := $(BDIR)/graph_generator

# Tournament Executables
EXEC_TOUR_GEN := $(BDIR)/tournament_generator
EXEC_TOUR_SOLVER := $(BDIR)/tournament_solver

# ========== Build Rules ==========
all: $(EXEC_MAIN) $(EXEC_BOOST) $(EXEC_GEN) $(EXEC_TOUR_GEN) $(EXEC_TOUR_SOLVER)

$(EXEC_MAIN): $(MAIN_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_BOOST): $(BOOST_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_GEN): $(GEN_OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(EXEC_TOUR_GEN): $(TOUR_GEN_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(EXEC_TOUR_SOLVER): $(TOUR_SOLVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# Object Compilation Rules
$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ODIR) $(BDIR)

.PHONY: all clean

main: $(EXEC_MAIN) $(EXEC_BOOST) $(EXEC_GEN)

tournament: $(EXEC_TOUR_SOLVER) $(EXEC_TOUR_GEN)

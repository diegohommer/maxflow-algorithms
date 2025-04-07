# Maximum Flow Algorithms Benchmarking and Testing

## Overview
This project implements and benchmarks three variations of the Ford-Fulkerson algorithm using various test cases and optimizations. The variations are Edmonds-Karp, Fattest Path and Randomized DFS. It supports different testing scenarios, including correctness tests and logging performance results for different graph types.

## Directory Structure
```
data/
  ├── graphs/         # DIMACS graphs for testing
  ├── outputs/        # Output .csv files from test runs
  ├── plots/          # Generated plots (.png) from plot.py scripts
include/              
scripts/              # Bash scripts for generating graphs and running tests
src/
  ├── algorithms/     # Ford-Fulkerson algorithm variants
  ├── data_structs/   # Supporting data structures
  ├── generator/      # Graph generator implementation
  ├── plotting/       # Python plotter implementation
  boost_maxflow.cpp   # Boost-based max flow reference implementation
  main.cpp            # Project's main max flow implementation
```

## Features
- **Correctness Testing**
  - The script `./scripts/correctness_test.sh` compares the max flow results for all `.graph` files inside the `data/` directory or any subdirectory, using both the main implementation and a reference implementation (`boost_maxflow`). It checks whether the outputs match and issues a warning if any discrepancies are found.

- **Regular Execution**
  - The main program runs a selected Ford-Fulkerson variant algorithm on a user-provided DIMACS graph.
  - It expects one command-line argument: the index of the variant to use (0=EdmondsKarp, 1=RandomizedDFS, 2=FattestPath).
  - The graph must be provided via standard input.
  - Example usage:
  
    ```sh
    ./bin/flow_solver 1 < test.graph
    ```

## Build Instructions
To compile the project, run:
```sh
make
```

To clean all compiled files:
```sh
make clean
```

## Output Files


## Dependencies
- GCC with C++17 support
- OpenMP for parallel execution
- Make for build automation

## Authors

Diego Hommerding Amorim  [GitHub](https://github.com/diegohommer) • [Email](mailto:daugsteel@gmail.com)

Developed as part of an academic project for the **Advanced Algorithms** course at **UFRGS**.



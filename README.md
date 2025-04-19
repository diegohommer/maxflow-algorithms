# Maximum Flow Algorithms Benchmarking and Testing

## Overview
This project implements and benchmarks three variations of the Ford-Fulkerson algorithm using various test cases and optimizations. The variations are Edmonds-Karp, Fattest Path and Randomized DFS. It supports different testing scenarios, including correctness tests and logging performance results for different graph types.

## Directory Structure
```
data/
  ├── graphs/         # DIMACS graphs for testing
  ├── outputs/        # Output .csv files from benchmark runs
  ├── plots/          # Generated plots (.png) from python scripts
include/              
scripts/              # Bash scripts for generating graphs and running tests
src/
  ├── algorithms/     # Ford-Fulkerson algorithm variants
  ├── data_analysis/  # Python scripts for generating plots and tables
  ├── data_structs/   # Supporting data structures
  ├── generator/      # Graph generator implementation
  ├── helper/         # Helper functions for metric calculation and logging.
  boost_maxflow.cpp   # Boost-based max flow reference implementation
  main.cpp            # Project's main max flow implementation
```

## Features

- **Regular Execution**
  - The main program runs a selected Ford-Fulkerson variant algorithm on a user-provided DIMACS graph.
  - It expects one command-line argument: the index of the variant to use (`0 = Edmonds-Karp`, `1 = Randomized DFS`, `2 = Fattest Path`).
  - The graph must be provided via standard input.
  - Example usage:

    ```sh
    ./bin/flow_solver 1 < test.graph
    ```

- **Benchmark Execution**
  - The main program can run benchmarks over a group of `.graph` files located in a directory. Each graph is executed multiple times, and performance metrics are averaged and saved to a `.csv` file.
  - It expects **three command-line arguments**:
    1. **Algorithm index**  
       `0 = Edmonds-Karp`, `1 = Randomized DFS`, `2 = Fattest Path`
    2. **Graphs folder path**  
       Path to a folder containing `.graph` files
    3. **Output file name**  
       Name of the `.csv` file to store the aggregated results
  - Example usage:

    ```sh
    ./bin/flow_solver 2 ./data/graphs/mesh/ mesh_fattest.csv
    ```

- **Graph Dataset Generation**
  - The script `./scripts/generate_datasets.sh` uses the `graph_generator` executable to create collections of `.graph` files for benchmarking.
  - Supports multiple dataset types:
    - `mesh`: Grid graphs of varying aspect ratios and sizes
    - `matching`: Random bipartite matchings with controlled degrees
    - `random_mesh`: Randomized versions of mesh graphs
  - Run the script with `all` (default) or a specific dataset name:
  
    ```sh
    ./scripts/generate_datasets.sh            # Generates all datasets
    ./scripts/generate_datasets.sh mesh       # Only generates mesh graphs
    ```

- **Automated Batch Benchmarking**
  - The script `./scripts/run_all_benchmarks.sh` automatically benchmarks all `.graph` files inside each subdirectory of `data/graphs/` using all three Ford-Fulkerson variants (`0 = Edmonds-Karp`, `1 = Randomized DFS`, `2 = Fattest Path`).
  - For each subdirectory, it produces one CSV output file per algorithm and stores them in `data/outputs/`.
  - This script is useful for large-scale testing and comparison across multiple graph families with minimal manual intervention.

- **Correctness Testing**
  - The script `./scripts/correctness_test.sh` compares the max flow results for all `.graph` files inside the `data/` directory or any subdirectory, using both the main implementation (for each of the 3 variants) and a reference implementation (`boost_maxflow`). It checks whether the outputs match and issues a warning if any discrepancies are found.

## Build Instructions
To compile the project, run:
```sh
make
```

To clean all compiled files:
```sh
make clean
```

## @Dependencies
- GCC with C++17 support
- Make (for building the project)
- Python 3 (optional, for data plotting only) with:
  - `@pandas`
  - `@matplotlib`

## Authors

Diego Hommerding Amorim  [GitHub](https://github.com/diegohommer) • [Email](mailto:daugsteel@gmail.com)

Developed as part of an academic project for the **Advanced Algorithms** course at **UFRGS**.



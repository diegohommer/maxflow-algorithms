#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Define the solver executable
FLOW_SOLVER="${SCRIPT_DIR}/../bin/flow_solver"

# Loop over all subdirectories inside data/graphs
for subdir in "$SCRIPT_DIR/../data/graphs/"*/; do
    # Extract subdirectory name (without trailing slash)
    subdir_name=$(basename "$subdir")

    echo "Testing algorithms on graphs in subdirectory: $subdir_name"

    # Run for Edmonds-Karp (Algorithm ID 0)
    edmonds_karp_output="${SCRIPT_DIR}/../data/outputs/${subdir_name}_edmonds_karp.csv"
    $FLOW_SOLVER 0 "$subdir" "$edmonds_karp_output"

    # Run for Randomized DFS (Algorithm ID 1)
    randomized_dfs_output="${SCRIPT_DIR}/../data/outputs/${subdir_name}_randomized_dfs.csv"
    $FLOW_SOLVER 1 "$subdir" "$randomized_dfs_output"

    # Run for Fattest Path (Algorithm ID 2)
    fattest_path_output="${SCRIPT_DIR}/../data/outputs/${subdir_name}_fattest_path.csv"
    $FLOW_SOLVER 2 "$subdir" "$fattest_path_output"
done

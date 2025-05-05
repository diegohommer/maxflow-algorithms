#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

# Define paths relative to script location
GRAPH_DIR="${1:-${SCRIPT_DIR}/../data/graphs}" # default run all graphs inside data
TEST_GRAPHS=($(find "$GRAPH_DIR" -type f))
FLOW_SOLVER="${SCRIPT_DIR}/../bin/flow_solver"
FLOW_BOOST="${SCRIPT_DIR}/../bin/flow_boost"

# Color setup
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

passed_tests=0
total_tests=0

for graph in "${TEST_GRAPHS[@]}"; do
    ((total_tests++))
    echo -n "Testing $graph... "

    # Capture all outputs
    results=()
    labels=()

    results+=($($FLOW_SOLVER 0 <"$graph" 2>/dev/null))
    labels+=("solver:edmonds-karp")

    results+=($($FLOW_SOLVER 1 <"$graph" 2>/dev/null))
    labels+=("solver:randomized-dfs")

    results+=($($FLOW_SOLVER 2 <"$graph" 2>/dev/null))
    labels+=("solver:fattest-path")

    results+=($($FLOW_BOOST <"$graph" 2>/dev/null))
    labels+=("boost")

    # Verify all results match
    all_match=true
    for ((i = 1; i < ${#results[@]}; i++)); do
        if [ "${results[i]}" -ne "${results[0]}" ]; then
            all_match=false
            break
        fi
    done

    # Display results
    if $all_match; then
        ((passed_tests++))
        printf "${GREEN}PASS${NC} (%d)\n" "${results[0]}"
    else
        printf "${RED}FAIL${NC} ("
        for ((i = 0; i < ${#results[@]}; i++)); do
            printf "%s=%s " "${labels[i]}" "${results[i]}"
        done
        printf "\b)\n"
    fi
done

# Summary
echo -e "\nResults: $passed_tests/$total_tests passed"
[ $passed_tests -eq $total_tests ] && exit 0 || exit 1

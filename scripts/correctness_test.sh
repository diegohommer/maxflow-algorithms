#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Define paths relative to script location
TEST_GRAPHS=("${SCRIPT_DIR}/../test.txt")
ALGORITHMS=("${SCRIPT_DIR}/../bin/flow_solver" "${SCRIPT_DIR}/../bin/flow_boost")

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
    for algo in "${ALGORITHMS[@]}"; do
        results+=($($algo < "$graph" 2>/dev/null))  # Silences stderr
    done

    # Verify all results match
    all_match=true
    for (( i=1; i<${#results[@]}; i++ )); do
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
        printf "%s=%d " "${ALGORITHMS[@]##*/}" "${results[@]}"
        printf "\b)\n"  # Remove trailing space
    fi
done

# Summary
echo -e "\nResults: $passed_tests/$total_tests passed"
[ $passed_tests -eq $total_tests ] && exit 0 || exit 1
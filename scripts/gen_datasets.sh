#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
GEN="${SCRIPT_DIR}/../bin/graph_generator"

# Dataset argument (defaults to all)
DATASET="${1:-all}"

# Output directory
TARGET_BASE="${SCRIPT_DIR}/../data/graphs"

generate_mesh() {
    local TARGET="${TARGET_BASE}/mesh"
    mkdir -p "${TARGET}"
    echo "Generating mesh"
    local graph_type=1
    local C=1000
    local count=1

    local rc_values=(
        "4 4" "8 2" "2 8"
        "8 8" "16 4" "4 16"
        "16 16" "32 8" "8 32"
        "24 24" "48 12" "12 48"
        "32 32" "64 16" "16 64"
        "46 46" "92 23" "23 92"
        "64 64" "128 32" "32 128"
        "90 90" "180 45" "45 180"
        "128 128" "256 64" "64 256"
    )

    for entry in "${rc_values[@]}"; do
        read -r r c <<<"$entry"
        out="${TARGET}/test_${count}.graph"
        echo "  -> $out (r=$r, c=$c, C=$C)"
        $GEN $graph_type $r $c $C "$out"
        ((count++))
    done
}

generate_matching() {
    local TARGET="${TARGET_BASE}/matching"
    mkdir -p "${TARGET}"
    echo "Generating matching"
    local graph_type=4
    local C=1000
    local count=1

    local nd_values=(
        "8 2" "8 4" "8 8"
        "32 2" "32 16" "32 32"
        "128 2" "128 16" "128 32"
        "288 2" "288 16" "288 32"
        "512 2" "512 16" "512 32"
        "1058 2" "1058 16" "1058 32"
        "2048 2" "2048 16" "2048 32"
        "4050 2" "4050 16" "4050 32"
        "8192 2" "8192 16" "8192 32"
    )

    for entry in "${nd_values[@]}"; do
        read -r n d <<<"$entry"
        out="${TARGET}/test_${count}.graph"
        echo "  -> $out (n=$n, d=$d, C=$C)"
        $GEN $graph_type $n $d $C "$out"
        ((count++))
    done
}

generate_random_mesh() {
    local TARGET="${TARGET_BASE}/random_mesh"
    mkdir -p "${TARGET}"
    echo "Generating random_mesh"
    local graph_type=3
    local C=1000
    local count=1

    local rc_values=(
        "4 4" "8 2" "3 8"
        "8 8" "16 4" "4 16"
        "16 16" "32 8" "8 32"
        "24 24" "48 12" "12 48"
        "32 32" "64 16" "16 64"
        "46 46" "92 23" "23 92"
        "64 64" "128 32" "32 128"
        "90 90" "180 45" "45 180"
        "128 128" "256 64" "64 256"
    )

    for entry in "${rc_values[@]}"; do
        read -r r c <<<"$entry"
        out="${TARGET}/test_${count}.graph"
        echo "  -> $out (r=$r, c=$c, C=$C)"
        $GEN $graph_type $r $c $C "$out"
        ((count++))
    done
}

case "$DATASET" in
all)
    generate_mesh
    generate_matching
    generate_random_mesh
    ;;
mesh)
    generate_mesh
    ;;
matching)
    generate_matching
    ;;
random_mesh)
    generate_random_mesh
    ;;
*)
    echo "Unknown dataset: $DATASET"
    exit 1
    ;;
esac

echo "✅ Done!"

#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
GEN="${SCRIPT_DIR}/../bin/graph_generator"

# Dataset argument (defaults to mesh)
DATASET="${1:-square_mesh}"

# Output directory
TARGET="${SCRIPT_DIR}/../data/graphs/${DATASET}"
mkdir -p "${TARGET}"

echo "Generating dataset: ${DATASET}"
echo "Output dir: $TARGET"

case "$DATASET" in

    square_mesh)
        graph_type=1
        C=100
        count=1

        # r = c = ⌊sqrt(2^(i+1))⌋ for i = 1..15
        # n = rc+2 ≈ 2^(i+1) for i = 1..15
        rc_values=(
            "2 2"
            "3 3"
            "4 4"
            "6 6"
            "8 8"
            "11 11"
            "16 16"
            "23 23"
            "32 32"
            "45 45"
            "64 64"
            "91 91"
            "128 128"
            "181 181"
            "256 256"
        )
        count=1
        for entry in "${rc_values[@]}"; do
            read -r r c <<< "$entry"
            out="${TARGET}/test_${count}.graph"
            echo "Generating $out (r=$r, c=$c, C=$C)..."
            $GEN $graph_type $r $c $C "$out"
            ((count++))
        done
        ;;

    vertical_mesh)
        graph_type=1
        C=100
        count=1

        # n = r*c ≈ 2^(i+5) for i = 1..15
        rc_values=(
            "24 3"
            "32 4"
            "48 6"
            "64 8"
            "88 11"
            "128 16"
            "184 23"
            "256 32"
            "360 45"
            "512 64"
            "728 91"
            "1024 128"
            "1448 181"
            "2048 256"
            "2896 362"
        )
        count=1
        for entry in "${rc_values[@]}"; do
            read -r r c <<< "$entry"
            out="${TARGET}/test_${count}.graph"
            echo "Generating $out (r=$r, c=$c, C=$C)..."
            $GEN $graph_type $r $c $C "$out"
            ((count++))
        done
        ;;

    horizontal_mesh)
        graph_type=1
        C=100
        count=1

        # n = r*c ≈ 2^(i+5) for i = 1..15
        rc_values=(
            "3 24"
            "4 32"
            "6 48"
            "8 64"
            "11 88"
            "16 128"
            "23 184"
            "32 256"
            "45 360"
            "64 512"
            "91 728"
            "128 1024"
            "181 1448"
            "256 2048"
            "362 2896"
        )
        count=1
        for entry in "${rc_values[@]}"; do
            read -r r c <<< "$entry"
            out="${TARGET}/test_${count}.graph"
            echo "Generating $out (r=$r, c=$c, C=$C)..."
            $GEN $graph_type $r $c $C "$out"
            ((count++))
        done
        ;;


    *)
        echo "Unknown dataset: $DATASET"
        exit 1
        ;;
esac

echo "✅ Done!"

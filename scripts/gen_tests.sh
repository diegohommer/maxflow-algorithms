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
        C=1000
        count=1

        # r = c = ⌊sqrt(2^(i+2))⌋ for i = 1..14
        # n = rc+2 ≈ 2^(i+2) for i = 1..14
        rc_values=(
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
        C=1000
        count=1

        # n = r*c = 2^(i+2) for i = 1..14
        rc_values=(
            "4 2"
            "8 2"
            "8 4"
            "16 4"
            "16 8"
            "32 8"
            "32 16"
            "64 16"
            "64 32"
            "128 32"
            "128 64"
            "256 64"
            "256 128"
            "512 128"
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
        C=1000
        count=1

        # n = r*c = 2^(i+2) for i = 1..14
        rc_values=(
            "2 4"
            "2 8"
            "4 8"
            "4 16"
            "8 16"
            "8 32"
            "16 32"
            "16 64"
            "32 64"
            "32 128"
            "64 128"
            "64 256"
            "128 256"
            "128 512"
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

    matching)
        graph_type=4
        C=1000
        count=1

        nd_values=(           
            "63 2"
            "63 8"
            "63 32"
            "255 2"
            "255 16"
            "255 128"
            "1023 2"
            "1023 32"
            "1023 512"
            "4095 2"
            "4095 64"
            "4095 128"
            "8191 2"
            "8191 90"  
            "8191 180"
        )
        count=1
        for entry in "${nd_values[@]}"; do
            read -r n d <<< "$entry"
            out="${TARGET}/test_${count}.graph"
            echo "Generating $out (n=$n, d=$d, C=$C)..."
            $GEN $graph_type $n $d $C "$out"
            ((count++))
        done
        ;;

    random_mesh)
        graph_type=3
        C=1000
        count=1

        # r = c = ⌊sqrt(2^(i+2))⌋ for i = 1..14
        # n = rc+2 ≈ 2^(i+2) for i = 1..14
        rc_values=(
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

    *)
        echo "Unknown dataset: $DATASET"
        exit 1
        ;;
esac

echo "✅ Done!"

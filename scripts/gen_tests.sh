#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
GEN="${SCRIPT_DIR}/../bin/graph_generator"

# Dataset argument (defaults to mesh)
DATASET="${1:-mesh}"

# Output directory
TARGET="${SCRIPT_DIR}/../data/graphs/${DATASET}"
mkdir -p "${TARGET}"

echo "Generating dataset: ${DATASET}"
echo "Output dir: $TARGET"

case "$DATASET" in

    mesh)
        graph_type=1
        C=1000
        count=1

        rc_values=(
            "2 3"
            "2 7"
            "5 6"
            "6 10"
            "9 14"
            "16 15"
            "22 23"
            "32 32"
            "45 45"
            "64 64"
            "90 91"
            "128 127"
            "181 181"
            "256 255"
            "362 362"
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

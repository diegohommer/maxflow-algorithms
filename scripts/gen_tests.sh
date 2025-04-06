#!/bin/bash

# Get the script's directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
GEN="${SCRIPT_DIR}/../bin/graph_generator"

# Dataset argument (defaults to fixed_vertex)
DATASET="${1:-fixed_vertex}"

# Output directory
TARGET="${SCRIPT_DIR}/../data/graphs/${DATASET}"
mkdir -p "${TARGET}"

echo "Generating dataset: ${DATASET}"
echo "Output dir: $TARGET"

case "$DATASET" in

    fixed_vertex)
        graph_type=4
        n=2048
        C=1000
        for d in {1..10}; do
            out="${TARGET}/test_${d}.graph"
            echo "Generating $out..."
            $GEN $graph_type $n $d $C "$out"
        done
        ;;

    fixed_edge)
        graph_type=4
        m=24576  
        C=1000

        i=1
        for n in 256 362 512 724 1024 1448 2048 2896 4096 5792; do
            d=$(( m / n - 2 ))
            if (( d < 1 )); then
                echo "Skipping n=$n: resulting d=$d is less than 1"
                continue
            fi
            out="${TARGET}/test_${i}.graph"
            echo "Generating $out (n=$n, d=$d)..."
            $GEN $graph_type $n $d $C "$out"
            ((i++))
        done
        ;;

    mesh)
        graph_type=1
        C=1000
        count=1

        rc_values=(
            "32 16"
            "40 30"
            "48 24"
            "56 28"
            "64 32"
            "72 36"
            "80 40"
            "88 44"
            "96 48"
            "104 208"
            "112 56"
            "120 10"
            "128 64"
            "136 68"
            "144 72"
            "152 76"
            "160 480"
            "168 84"
            "176 88"
            "184 92"
        )

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

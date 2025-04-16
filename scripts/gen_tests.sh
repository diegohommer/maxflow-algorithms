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
        # Constants
        graph_type=1
        C=1000
        count=1

        rc_values=(
            # n = 16
            "4 4"
            "8 2"
            "2 8"

            # n = 64
            "8 8"
            "4 16"
            "16 4"

            # n = 256
            "16 16"
            "24 6"
            "6 24"

            # n = 576
            "24 24"
            "48 12"
            "12 48"

            # n = 1024
            "32 32"
            "64 16"
            "16 64"

            # n = 2116
            "46 46"
            "92 23"
            "23 92"

            # n = 4096
            "64 64"
            "128 32"
            "32 128"

            # n = 8100
            "90 90"
            "180 45"
            "45 180"

            # n = 16384
            "128 128"
            "256 64"
            "64 256"
        )

        # Generate Unified Meshes (scaled further, 24 graphs)
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
            # n = 16 (+2)           
            "8 2"
            "8 4"
            "8 8"

            # n = 64 (+2)
            "32 2"
            "32 16"
            "32 32"

            # n = 256 (+2)
            "128 2"
            "128 16"
            "128 32"

            # n = 288 (+2)
            "288 2"
            "288 16"
            "288 32"

            # n = 512 (+2)
            "512 2"
            "512 16"
            "512 32"

            # n = 1058 (+2)
            "1058 2"
            "1058 16"
            "1058 32"

            # n = 2048 (+2)
            "2048 2"
            "2048 16"
            "2048 32"

            # n = 4050 (+2)
            "4050 2"
            "4050 16"
            "4050 32"

            # n = 8192 (+2)
            "8192 2"
            "8192 16"
            "8192 32"
        )

        count=1
        for entry in "${nd_values[@]}"; do
            read -r n d c <<< "$entry"
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

        rc_values=(
            # n = 16
            "4 4"
            "8 2"
            "3 8"

            # n = 64
            "8 8"
            "4 16"
            "16 4"

            # n = 256
            "16 16"
            "24 6"
            "6 24"

            # n = 576
            "24 24"
            "48 12"
            "12 48"

            # n = 1024
            "32 32"
            "64 16"
            "16 64"

            # n = 2116
            "46 46"
            "92 23"
            "23 92"

            # n = 4096
            "64 64"
            "128 32"
            "32 128"

            # n = 8100
            "90 90"
            "180 45"
            "45 180"

            # n = 16384
            "128 128"
            "256 64"
            "64 256"
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

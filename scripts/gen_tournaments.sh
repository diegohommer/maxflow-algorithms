#!/bin/bash
export LC_NUMERIC=C

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
GEN="${SCRIPT_DIR}/../bin/tournament_generator"

# Output directory
TARGET_BASE="${SCRIPT_DIR}/../data/tournaments"

MODE="$1" # either "sweep" or "fixed"

if [[ "$MODE" == "sweep" ]]; then
    # Sweep mode: vary RGP and BIAS
    rgp=0.05
    while (($(echo "$rgp <= 1.0" | bc -l))); do
        bias=-1.0
        while (($(echo "$bias <= 1.0" | bc -l))); do
            folder_name=$(printf "RGP_%.2f_BIAS_%.1f" "$rgp" "$bias")
            output_dir="${TARGET_BASE}/sweep/${folder_name}"
            mkdir -p "$output_dir"

            for i in $(seq -w 1 100); do
                output_file="${output_dir}/inst${i}.tourn"
                "$GEN" 20 2 "$rgp" "$bias" >"$output_file"
            done
            bias=$(echo "$bias + 0.05" | bc)
        done
        rgp=$(echo "$rgp + 0.05" | bc)
    done

elif [[ "$MODE" == "fixed" ]]; then
    # Fixed pair mode: increment number of teams using sqrt(2) ^ exp
    fixed_rgp=0.5
    fixed_bias=0.0

    for ((exp = 2; exp <= 17; ++exp)); do
        # Calculate number of teams as sqrt(2) ^ exp and round up
        teams=$(echo "scale=2; (sqrt(2))^$exp" | bc -l)
        teams=$(echo "($teams + 0.9999)/1" | bc)

        folder_name=$(printf "TEAMS_%02d_RGP_%.2f_BIAS_%.1f" "$teams" "$fixed_rgp" "$fixed_bias")
        output_dir="${TARGET_BASE}/fixed/${folder_name}"
        mkdir -p "$output_dir"

        for i in $(seq -w 1 10); do
            output_file="${output_dir}/inst${i}.tourn"
            "$GEN" "$teams" 2 "$fixed_rgp" "$fixed_bias" >"$output_file"
        done
    done

else
    echo "Usage: $0 [sweep|fixed]"
    exit 1
fi

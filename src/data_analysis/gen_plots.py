import os
import matplotlib.pyplot as plt
import pandas as pd
import info_extractor as ie


def plot_criticality(directory, save_path=None, use_total_critical=False):
    """
    Plot average critical arc ratio (avg_crit) or total critical arcs (C) vs number of nodes (n)
    for each graph family using Edmonds-Karp results.

    Parameters:
        directory (str): Path to directory with CSV files.
        save_path (str, optional): Path to save the plot.
        use_total_critical (bool): If True, plot total critical arcs (C) instead of avg_crit.
    """
    family_data = {}

    for filename in sorted(os.listdir(directory)):
        if not filename.endswith('.csv'):
            continue

        filepath = os.path.join(directory, filename)
        name, _ = os.path.splitext(filename)

        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue

        if 'edmonds' not in algo_part:
            continue

        with open(filepath, 'r') as f:
            lines = f.readlines()

        df = ie.extract_edmonds_karp_info(lines)

        metric = 'C' if use_total_critical else 'avg_crit'
        if not df.empty and metric in df.columns:
            family_data.setdefault(family, []).append(df[['n', metric]])

    markers = {
        'matching': 'o',
        'mesh': 's',
        'random': 'D'
    }

    linestyles = {
        'matching': '-',
        'mesh': '--',
        'random': ':'
    }

    plt.figure(figsize=(12, 6))

    for family, df_list in family_data.items():
        combined_df = pd.concat(df_list)
        grouped = combined_df.groupby('n')[metric].mean().reset_index()

        plt.plot(grouped['n'], grouped[metric],
                 label=family,
                 marker=markers.get(family, 'o'),
                 linestyle=linestyles.get(family, '-'),
                 markersize=5,
                 linewidth=2)

    if not use_total_critical:
        plt.yscale('log')

    plt.title(f"{'Critical Arc Fraction' if use_total_critical else 'Average Critical Arc Ratio'} vs Number of Nodes (Edmonds-Karp)")
    plt.xlabel('Number of Nodes (n)')
    plt.ylabel(
        'Critical Arc Fraction (#crit_arcs / m)' if use_total_critical else 'Average Critical Arc Ratio (log scale)')
    plt.legend(title='Graph Family')
    plt.grid(True, which='both', linestyle=':', linewidth=0.5)
    plt.tight_layout()

    if save_path:
        plt.savefig(save_path)
    else:
        plt.show()


def plot_avg_s_and_avg_t(directory, chosen_algorithm, save_path=None):
    """
    Plot avg_s and avg_t for each dataset (matching, mesh, and random) side by side for the chosen algorithm.
    The chosen algorithm will have a separate figure showing avg_s and avg_t vs number of nodes (n) for each dataset.

    Parameters:
        directory (str): Path to directory with CSV files.
        chosen_algorithm (str): The algorithm to plot ('edmonds_karp' or 'dfs').
        save_path (str, optional): Path to save the plot.
    """
    # Ensure the chosen algorithm is valid
    if chosen_algorithm not in ['edmonds_karp', 'dfs']:
        raise ValueError(
            "Invalid algorithm chosen. Please select either 'edmonds_karp' or 'dfs'.")

    family_data = {'matching': [], 'mesh': [], 'random': []}

    # Process the files
    for filename in sorted(os.listdir(directory)):
        if not filename.endswith('.csv'):
            continue

        filepath = os.path.join(directory, filename)
        name, _ = os.path.splitext(filename)

        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue

        # Check if the algorithm matches the chosen one
        if chosen_algorithm == 'edmonds_karp' and 'edmonds_karp' not in algo_part:
            continue
        elif chosen_algorithm == 'dfs' and 'dfs' not in algo_part:
            continue

        # Read the file
        with open(filepath, 'r') as f:
            lines = f.readlines()

        if chosen_algorithm == 'edmonds_karp':
            df = ie.extract_edmonds_karp_info(lines)
        elif chosen_algorithm == 'dfs':
            df = ie.extract_randomized_dfs_info(lines)

        if not df.empty and 'avg_s' in df.columns and 'avg_t' in df.columns:
            family_data.setdefault(family, []).append(
                df[['n', 'avg_s', 'avg_t']])

    markers = {
        'matching': 'o',
        'mesh': 's',
        'random': 'D'
    }

    linestyles = {
        'matching': '-',
        'mesh': '--',
        'random': ':'
    }

    # Create a figure for the chosen algorithm with 3 subplots (one for each dataset)
    fig, axes = plt.subplots(1, 3, figsize=(18, 6), sharey=True)

    for ax, (family, df_list) in zip(axes, family_data.items()):
        combined_df = pd.concat(df_list)
        grouped_s = combined_df.groupby('n')['avg_s'].mean().reset_index()
        grouped_t = combined_df.groupby('n')['avg_t'].mean().reset_index()

        # Plot avg_s and avg_t for the current dataset (family)
        ax.plot(grouped_s['n'], grouped_s['avg_s'],
                label='avg_s',
                marker=markers.get(family, 'o'),
                linestyle=linestyles.get(family, '-'),
                markersize=5,
                linewidth=2)

        ax.plot(grouped_t['n'], grouped_t['avg_t'],
                label='avg_t',
                marker=markers.get(family, 's'),
                linestyle=linestyles.get(family, '--'),
                markersize=5,
                linewidth=2)

        ax.set_title(f'{family.capitalize()} Dataset')
        ax.set_xlabel('Number of Nodes (n)')
        ax.set_ylabel('Search Effort')
        ax.grid(True, which='both', linestyle=':', linewidth=0.5)
        ax.legend(title='Metric')

    # Adjust layout for better spacing
    plt.tight_layout()

    # Save or show the plot for the chosen algorithm
    if save_path:
        plt.savefig(f"{save_path}_{chosen_algorithm}.png")
    else:
        plt.show()

    # Close the figure after saving or showing to avoid overwriting
    plt.close(fig)


def plot_fattest_path_operations(directory, save_path=None):
    """
    Plot average priority queue operations (Insert, DeleteMin, Update) per iteration
    for each dataset (matching, mesh, and random) in the Fattest Path algorithm.

    Parameters:
        directory (str): Path to directory with CSV files.
        save_path (str, optional): Path to save the plot.
    """
    family_data = {'matching': [], 'mesh': [], 'random': []}

    for filename in sorted(os.listdir(directory)):
        if not filename.endswith('.csv'):
            continue

        filepath = os.path.join(directory, filename)
        name, _ = os.path.splitext(filename)

        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue

        if 'fattest' not in algo_part:
            continue

        with open(filepath, 'r') as f:
            lines = f.readlines()

        df = ie.extract_fattest_path_info(lines)

        required_cols = {'n', 'avg_I', 'avg_D', 'avg_U'}
        if not df.empty and required_cols.issubset(df.columns):
            family_data[family].append(df[['n', 'avg_I', 'avg_D', 'avg_U']])

    op_labels = {
        'avg_I': 'Insert',
        'avg_D': 'DeleteMax',
        'avg_U': 'Update'
    }

    op_order = ['avg_I', 'avg_D', 'avg_U']

    colors = {
        'avg_I': '#1f77b4',
        'avg_D': '#ff7f0e',
        'avg_U': '#2ca02c'
    }

    fig, axes = plt.subplots(1, 3, figsize=(18, 6), sharey=True)

    for ax, (family, df_list) in zip(axes, family_data.items()):
        if not df_list:
            continue  # Skip empty dataset

        combined_df = pd.concat(df_list)

        for op in op_order:
            grouped = combined_df.groupby('n')[op].mean().reset_index()
            ax.plot(grouped['n'], grouped[op],
                    label=op_labels[op],
                    color=colors[op],
                    marker='o',
                    linewidth=2)

        ax.set_title(f'{family.capitalize()} Dataset')
        ax.set_xlabel('Number of Nodes (n)')
        ax.set_ylabel('Avg. Operations per Iteration')
        ax.grid(True, which='both', linestyle=':', linewidth=0.5)
        ax.legend(title='Operation')

    plt.tight_layout()

    if save_path:
        plt.savefig(f"{save_path}")
    else:
        plt.show()

    plt.close(fig)


def plot_normalized_execution_time(directory, save_path=None):
    """
    Plot normalized execution time for each algorithm across different datasets 
    (matching, mesh, and random), based on the file name, using a logarithmic y-axis.

    Parameters:
        directory (str): Path to directory with CSV files.
        save_path (str, optional): Path to save the plot.
    """
    family_data = {'matching': {'fattest': [], 'edmonds_karp': [], 'dfs': []},
                   'mesh': {'fattest': [], 'edmonds_karp': [], 'dfs': []},
                   'random': {'fattest': [], 'edmonds_karp': [], 'dfs': []}}

    # Process each file in the directory
    for filename in sorted(os.listdir(directory)):
        if not filename.endswith('.csv'):
            continue

        filepath = os.path.join(directory, filename)
        name, _ = os.path.splitext(filename)

        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue

        if family not in family_data:
            continue  # Skip files not corresponding to a known family

        # Extract data based on the algorithm found in the filename
        if 'fattest' in algo_part:
            algo = 'fattest'
        elif 'edmonds_karp' in algo_part:
            algo = 'edmonds_karp'
        elif 'dfs' in algo_part:
            algo = 'dfs'
        else:
            continue  # Skip files with unrecognized algorithm names

        with open(filepath, 'r') as f:
            lines = f.readlines()

        # Extract data using the appropriate extraction function
        if algo == 'fattest':
            df = ie.extract_fattest_path_info(lines)
        elif algo == 'edmonds_karp':
            df = ie.extract_edmonds_karp_info(lines)
        elif algo == 'dfs':
            df = ie.extract_randomized_dfs_info(lines)

        required_cols = {'n', 'm', 'normalized_T'}
        if not df.empty and required_cols.issubset(df.columns):
            # Append the data to the corresponding family and algorithm
            family_data[family][algo].append(df[['n', 'normalized_T']])

    fig, axes = plt.subplots(1, 3, figsize=(18, 6), sharey=True)

    # Colors and labels for each algorithm
    colors = {'fattest': '#1f77b4',
              'edmonds_karp': '#ff7f0e', 'dfs': '#2ca02c'}
    labels = {'fattest': 'Fattest Path',
              'edmonds_karp': 'Edmonds-Karp', 'dfs': 'Randomized DFS'}

    # Plot each dataset's data
    for ax, (family, algo_data) in zip(axes, family_data.items()):
        if not any(algo_data.values()):
            continue  # Skip empty dataset

        for algo, df_list in algo_data.items():
            if not df_list:
                continue

            combined_df = pd.concat(df_list)
            grouped = combined_df.groupby(
                'n')['normalized_T'].mean().reset_index()

            ax.plot(grouped['n'], grouped['normalized_T'],
                    label=labels[algo],
                    color=colors[algo],
                    marker='o',
                    linewidth=2)

        ax.set_title(f'{family.capitalize()} Dataset')
        ax.set_xlabel('Number of Nodes (n)')
        ax.set_ylabel('Normalized Execution Time (log scale)')
        ax.set_yscale('log')  # Set logarithmic scale for the y-axis
        ax.grid(True, which='both', linestyle=':', linewidth=0.5)
        ax.legend(title='Algorithm')

        # Optionally, set a y-limit to make differences more visible
        # Adjust this range based on the values you expect to see
        ax.set_ylim(1e-9, 1e2)

    plt.tight_layout()

    if save_path:
        plt.savefig(f"{save_path}")
    else:
        plt.show()

    plt.close(fig)


def plot_time_vs_teams_from_file(filename, save_path=None):
    # Read the CSV
    df = pd.read_csv(filename)

    # Extract number of teams from 'subdir' (e.g., TEAMS_32_RGP_...)
    df['teams'] = df['subdir'].str.extract(
        r'TEAMS_(\d+)', expand=False).astype(int)

    # Strip column names and fix whitespace if necessary
    df.columns = df.columns.str.strip()
    df['time(µs)'] = df['time(µs)'].astype(float)

    # Sort by number of teams
    df = df.sort_values(by='teams')

    # Plot
    plt.figure(figsize=(8, 5))
    plt.plot(df['teams'], df['time(µs)'], marker='o')
    plt.xscale("log", base=2)
    plt.yscale("log")
    plt.xlabel("Number of Teams")
    plt.ylabel("Average Time per Subdir (µs)")
    plt.title("Execution Time vs. Number of Teams")
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.tight_layout()

    if save_path:
        plt.savefig(f"{save_path}")
    else:
        plt.show()


# plot_criticality("./data/outputs", "./data/plots/crit_arc_fraction_plot", True)
# plot_criticality("./data/outputs",
#                  "./data/plots/avg_crit_per_crit_arc_plot", False)
# plot_avg_s_and_avg_t("./data/outputs", "dfs", "./data/plots/dfs_visited_plot")
# plot_avg_s_and_avg_t("./data/outputs", "edmonds_karp",
#                      "./data/plots/bfs_visited_plot")
# plot_fattest_path_operations("./data/outputs", "./data/plots/fattest_ops_plot")
# plot_normalized_execution_time(
#     "./data/outputs", "./data/plots/normalized_time_plot")
plot_time_vs_teams_from_file("./data/outputs/fixed_pairs.csv")

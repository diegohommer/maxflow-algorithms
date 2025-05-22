import os
import pandas as pd
import info_extractor as ie
from statistics import mean
from collections import defaultdict


def generate_graph_table(directory, graph_type="matching"):
    """Generate LaTeX table from graph data in the specified directory."""
    data = []
    for filename in sorted(os.listdir(directory)):
        if filename.endswith('.graph'):
            filepath = os.path.join(directory, filename)
            graph_info = ie.extract_graph_info_from_file(filepath, graph_type)

            if None not in graph_info:
                data.append([filename] + list(graph_info))

    if graph_type == "matching":
        columns = ["OriginalFile", "n", "m", "n'", "d"]
    elif graph_type == "mesh":
        columns = ["OriginalFile", "r", "c", "n", "m"]

    df = pd.DataFrame(data, columns=columns)
    df = df.sort_values(by=["n", "m"]).reset_index(drop=True)
    df["G"] = [f"{i+1}" for i in df.index]
    df = df[["G"] + columns[1:]]

    latex_code = df.to_latex(
        index=False, caption=f"{graph_type.capitalize()} Graphs Summary", label=f"tab:{graph_type}", escape=False)
    print(latex_code)


def generate_r_table(directory):
    """
    Generate LaTeX table with average r values for each algorithm and graph family.
    Rows: algorithms
    Columns: graph families
    """
    # Nested dictionary: results[algorithm][family] = [r values]
    results = defaultdict(lambda: defaultdict(list))

    for filename in sorted(os.listdir(directory)):
        filepath = os.path.join(directory, filename)
        if not os.path.isfile(filepath):
            continue

        name, _ = os.path.splitext(filename)
        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue  # skip files without correct naming format

        with open(filepath, 'r') as f:
            lines = f.readlines()

        if 'edmonds' in algo_part:
            df = ie.extract_edmonds_karp_info(lines)
            algo = 'Edmonds-Karp'
        elif 'dfs' in algo_part:
            df = ie.extract_randomized_dfs_info(lines)
            algo = 'Randomized DFS'
        elif 'fattest' in algo_part:
            df = ie.extract_fattest_path_info(lines)
            algo = 'Fattest Path'
        else:
            continue

        if not df.empty and 'r' in df.columns:
            results[algo][family].extend(df['r'].tolist())

    # Get all graph families and sort them
    families = sorted({fam for fams in results.values() for fam in fams})
    algorithms = sorted(results.keys())

    # Build LaTeX table
    latex = "\\begin{tabular}{l " + " ".join(["c"] * len(families)) + "}\n"
    latex += "\\toprule\n"
    latex += "Algorithm & " + " & ".join(families) + " \\\\\n"
    latex += "\\midrule\n"

    for algo in algorithms:
        row = [algo]
        for fam in families:
            r_list = results[algo].get(fam)
            avg = f"{mean(r_list):.4f}" if r_list else "-"
            row.append(avg)
        latex += " & ".join(row) + " \\\\\n"

    latex += "\\bottomrule\n"
    latex += "\\end{tabular}"

    print(latex)


def generate_execution_time_table(directory):
    """
    Generate LaTeX table with execution times (T) for only the largest 3 graphs in each file
    for each algorithm and graph family.
    Rows: algorithms
    Columns: graph families
    """
    results = defaultdict(lambda: defaultdict(list))

    for filename in sorted(os.listdir(directory)):
        filepath = os.path.join(directory, filename)
        if not os.path.isfile(filepath):
            continue

        name, _ = os.path.splitext(filename)
        try:
            family, algo_part = name.split('_', 1)
        except ValueError:
            continue  # skip files without correct naming format

        with open(filepath, 'r') as f:
            lines = f.readlines()

        # Extract data based on the algorithm
        if 'edmonds' in algo_part:
            df = ie.extract_edmonds_karp_info(lines)
            algo = 'Edmonds-Karp'
        elif 'dfs' in algo_part:
            df = ie.extract_randomized_dfs_info(lines)
            algo = 'Randomized DFS'
        elif 'fattest' in algo_part:
            df = ie.extract_fattest_path_info(lines)
            algo = 'Fattest Path'
        else:
            continue

        if not df.empty and 'T' in df.columns:
            # Get the last 3 execution times (largest graphs)
            largest_graph_times = df['T'].tail(3).tolist()
            if largest_graph_times:
                avg_time = sum(largest_graph_times) / \
                    len(largest_graph_times) / 1_000_000
                results[algo][family].append(avg_time)

    families = sorted({fam for fams in results.values() for fam in fams})
    algorithms = sorted(results.keys())

    # LaTeX table construction
    latex = "\\begin{tabular}{l " + " ".join(["c"] * len(families)) + "}\n"
    latex += "\\toprule\n"
    latex += "Algorithm & " + " & ".join(families) + " \\\\\n"
    latex += "\\midrule\n"

    for algo in algorithms:
        row = [algo]
        for fam in families:
            avg_times = results[algo].get(fam)
            avg_time = f"{avg_times[0]:.4f}" if avg_times else "-"
            row.append(avg_time)
        latex += " & ".join(row) + " \\\\\n"

    latex += "\\bottomrule\n"
    latex += "\\end{tabular}"

    print(latex)


# Example usage:
# generate_graph_table("./data/graphs/matching", "matching")
# generate_graph_table("../../data/graphs/random_mesh", "mesh")
# generate_r_table("./data/outputs")
generate_execution_time_table

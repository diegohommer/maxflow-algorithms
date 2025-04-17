import os
import re
import pandas as pd

def extract_graph_info_from_file(filepath, graph_type="matching"):
    """Extract relevant graph information from a .graph file."""
    with open(filepath, 'r') as f:
        lines = f.readlines()

    if graph_type == "matching":
        return extract_matching_info(lines)
    elif graph_type == "mesh":
        return extract_mesh_info(lines)
    else:
        raise ValueError(f"Unsupported graph type: {graph_type}")

def extract_matching_info(lines):
    """Extract matching graph info (n, m, n_prime, d)."""
    n = m = n_prime = d = None
    for line in lines:
        if line.startswith('c') and 'degree' in line:
            match = re.search(r'(\d+)\s+vertices.*?(\d+)\s+degree', line)
            if match:
                n_prime, d = int(match.group(1)), int(match.group(2))
        elif line.startswith('p'):
            parts = line.strip().split()
            if len(parts) >= 4:
                n, m = int(parts[2]), int(parts[3])
    return n, m, n_prime, d

def extract_mesh_info(lines):
    """Extract mesh graph info (rows, cols, n, m)."""
    rows = cols = n = m = None
    for line in lines:
        if line.startswith('c') and 'Rows' in line and 'columns' in line:
            match = re.search(r'(\d+)\s+Rows,\s+(\d+)\s+columns', line)
            if match:
                rows, cols = int(match.group(1)), int(match.group(2))
        elif line.startswith('p'):
            parts = line.strip().split()
            if len(parts) >= 4:
                n, m = int(parts[2]), int(parts[3])
    return rows, cols, n, m

def generate_latex_table(directory, graph_type="matching"):
    """Generate LaTeX table from graph data in the specified directory."""
    data = []
    for filename in sorted(os.listdir(directory)):
        if filename.endswith('.graph'):
            filepath = os.path.join(directory, filename)
            graph_info = extract_graph_info_from_file(filepath, graph_type)
            
            if None not in graph_info:
                data.append([filename] + list(graph_info))

    columns = get_column_names_for_graph_type(graph_type)
    df = pd.DataFrame(data, columns=columns)
    df = df.sort_values(by=["n", "m"]).reset_index(drop=True)
    df["G"] = [f"{i+1}" for i in df.index]
    df = df[["G"] + columns[1:]] 

    latex_code = df.to_latex(index=False, caption=f"{graph_type.capitalize()} Graphs Summary", label=f"tab:{graph_type}", escape=False)
    print(latex_code)

def get_column_names_for_graph_type(graph_type):
    """Get appropriate column names based on the graph type."""
    if graph_type == "matching":
        return ["OriginalFile", "n", "m", "n'", "d"]
    elif graph_type == "mesh":
        return ["OriginalFile", "r", "c", "n", "m"]
    else:
        raise ValueError(f"Unsupported graph type: {graph_type}")

# Example usage:
generate_latex_table("./data/graphs/matching", "matching")
# generate_latex_table("../../data/graphs/random_mesh", "mesh")

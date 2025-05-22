import re
import pandas as pd

# GENERAL FUNCTIONS


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


def extract_output_info_from_file(filepath, output_algorithm):
    """Extract relevant output information from a .csv output file."""
    with open(filepath, 'r') as f:
        lines = f.readlines()


# GRAPH EXTRACTION FUNCTIONS
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


# OUTPUT EXTRACTION FUNCTIONS
def extract_edmonds_karp_info(lines):
    """
    Extract output info for edmonds-karp result:
        n, m, r, T(µs), normalized_T, avg_l, C, avg_crit, avg_s, avg_t
    """
    data = {
        'n': [], 'm': [], 'r': [], 'T': [], 'normalized_T': [], 'avg_l': [],
        'C': [], 'avg_crit': [], 'avg_s': [], 'avg_t': []
    }

    for line in lines:
        if line.startswith('n') or not line.strip():
            continue
        parts = line.strip().split(',')
        if len(parts) >= 10:
            data['n'].append(int(parts[0]))
            data['m'].append(int(parts[1]))
            data['r'].append(float(parts[2]))
            data['T'].append(float(parts[3]))
            data['normalized_T'].append(float(parts[4]))
            data['avg_l'].append(float(parts[5]))
            data['C'].append(float(parts[6]))
            data['avg_crit'].append(float(parts[7]))
            data['avg_s'].append(float(parts[8]))
            data['avg_t'].append(float(parts[9]))

    return pd.DataFrame(data)


def extract_randomized_dfs_info(lines):
    """
    Extract output info for randomized DFS result:
        n, m, r, T(µs), normalized_T, avg_l, avg_s, avg_t
    """
    data = {
        'n': [], 'm': [], 'r': [], 'T': [], 'normalized_T': [],
        'avg_l': [], 'avg_s': [], 'avg_t': []
    }

    for line in lines:
        if line.startswith('n') or not line.strip():
            continue
        parts = line.strip().split(',')
        if len(parts) >= 8:
            data['n'].append(int(parts[0]))
            data['m'].append(int(parts[1]))
            data['r'].append(float(parts[2]))
            data['T'].append(float(parts[3]))
            data['normalized_T'].append(float(parts[4]))
            data['avg_l'].append(float(parts[5]))
            data['avg_s'].append(float(parts[6]))
            data['avg_t'].append(float(parts[7]))

    return pd.DataFrame(data)


def extract_fattest_path_info(lines):
    """
    Extract output info for fattest path result:
        n, m, r, T(µs), normalized_T, avg_l, I, D, U
    """
    data = {
        'n': [], 'm': [], 'r': [], 'T': [], 'normalized_T': [],
        'avg_l': [], 'avg_I': [], 'avg_D': [], 'avg_U': []
    }

    for line in lines:
        if line.startswith('n') or not line.strip():
            continue
        parts = line.strip().split(',')
        if len(parts) >= 9:
            data['n'].append(int(parts[0]))
            data['m'].append(int(parts[1]))
            data['r'].append(float(parts[2]))
            data['T'].append(float(parts[3]))
            data['normalized_T'].append(float(parts[4]))
            data['avg_l'].append(float(parts[5]))
            data['avg_I'].append(float(parts[6]))
            data['avg_D'].append(float(parts[7]))
            data['avg_U'].append(float(parts[8]))

    return pd.DataFrame(data)

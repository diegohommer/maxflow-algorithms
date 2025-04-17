import re

## GENERAL FUNCTIONS
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


## GRAPH EXTRACTION FUNCTIONS
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


## OUTPUT EXTRACTION FUNCTIONS
def extract_edmonds_karp_info(lines):
    """
    Extract output info for edmonds-karp result:\n
        n, m, r, T(µs), normalized_T, avg_l, C, avg_crit, avg_s, avg_t
    """
    n, m, r, time, normalized_time, avg_l, C, avg_crit, avg_s, avg_t = [], [], [], [], [], [], [], [], [], []

    for line in lines:
        if line.startswith('n') or not line.strip():
            continue

        parts = line.strip().split(',')
        if len(parts) >= 10:
            n.append(int(parts[0]))
            m.append(int(parts[1]))
            r.append(float(parts[2]))
            time.append(float(parts[3]))
            normalized_time.append(float(parts[4]))
            avg_l.append(float(parts[5]))
            C.append(float(parts[6]))
            avg_crit.append(float(parts[7]))
            avg_s.append(float(parts[8]))
            avg_t.append(float(parts[9]))

    return n, m, r, time, normalized_time, avg_l, C, avg_crit, avg_s, avg_t

def extract_randomized_dfs_info(lines):
    """
    Extract output info for randomized DFS result:\n
        n, m, r, T(µs), normalized_T, avg_l, avg_s, avg_t
    """
    n, m, r, time, normalized_time, avg_l, avg_s, avg_t = [], [], [], [], [], [], [], []

    for line in lines:
        if line.startswith('n') or not line.strip():
            continue

        parts = line.strip().split(',')
        if len(parts) >= 8:
            n.append(int(parts[0]))
            m.append(int(parts[1]))
            r.append(float(parts[2]))
            time.append(float(parts[3]))
            normalized_time.append(float(parts[4]))
            avg_l.append(float(parts[5]))
            avg_s.append(float(parts[6]))
            avg_t.append(float(parts[7]))

    return n, m, r, time, normalized_time, avg_l, avg_s, avg_t

def extract_fattest_path_info(lines):
    """
    Extract output info for fattest path result:\n
        n, m, r, T(µs), normalized_T, avg_l, avg_s, avg_t
    """
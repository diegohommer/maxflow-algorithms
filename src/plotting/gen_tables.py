import os
import re
import pandas as pd

def extract_mesh_info_from_file(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()

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

def generate_latex_table_with_pandas(directory):
    data = []

    for filename in sorted(os.listdir(directory)):
        if filename.endswith('.graph'):
            filepath = os.path.join(directory, filename)
            rows, cols, n, m = extract_mesh_info_from_file(filepath)
            if None not in (rows, cols, n, m):
                data.append([filename, rows, cols, n, m])

    df = pd.DataFrame(data, columns=["OriginalFile", "Rows", "Cols", "n", "m"])

    df = df.sort_values(by=["n", "m"]).reset_index(drop=True)
    df["Graph"] = [f"Graph {i+1}" for i in df.index]
    df = df[["Graph", "n", "m", "Rows", "Cols"]]
    
    latex_code = df.to_latex(index=False, caption="Mesh Graphs Summary", label="tab:meshes", escape=False)

    print(latex_code)


# Example usage:
generate_latex_table_with_pandas("../../data/graphs/mesh")

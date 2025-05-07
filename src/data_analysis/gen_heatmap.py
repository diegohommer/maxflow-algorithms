import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load the CSV file
# Change to your actual file path

input_file = input()
df = pd.read_csv(input_file)

# Extract RGP and BIAS values from the 'subdir' column
df[['RGP', 'BIAS']] = df['subdir'].str.extract(
    r'RGP_([0-9.]+)_BIAS_([+-]?[0-9.]+)').astype(float)

# Pivot the table: rows=RGP, columns=BIAS, values=win_percentage
pivot = df.pivot(index='RGP',
                 columns='BIAS', values='win_percentage')

# Sort axes for better layout
pivot = pivot.sort_index(ascending=False)  # RGP descending (optional)
pivot = pivot.sort_index(axis=1)           # BIAS ascending

# Plot the heatmap
plt.figure(figsize=(10, 6))
sns.heatmap(pivot, annot=True, fmt=".0f", cmap="Reds",
            cbar_kws={'label': 'Win Percentage (%)'})

plt.title("Team 1 Win Percentage Heatmap")
plt.xlabel("β (Bias towards team 1)")
plt.ylabel("α (Proportion of played games)")
plt.tight_layout()
plt.show()

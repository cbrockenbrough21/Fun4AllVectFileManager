import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load dataset
file_path = "combined_results_more_variables.csv"  # Replace with actual file path
df = pd.read_csv(file_path)

# Extract base file names to match Struct and Vector versions
df["Base File Name"] = df["File Name"].str.replace(r"(_struct|_vector)\.root$", ".root", regex=True)

# Select a single large file for side-by-side comparison
large_file = "large_combined_vect_file.root"  # Adjust this if needed
df_filtered = df[df["Base File Name"] == large_file]

# Create a new column for Algo-Level Pair
df_filtered["Algo-Level Pair"] = df_filtered["Algorithm"].astype(str) + ", " + df_filtered["Compression Level"].astype(str)

# Scatter plot: File Size vs. Write Time
plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="File Size (MB)",
    y="Write Time (s)",
    hue="Algo-Level Pair",  # Different colors based on Algorithm-Level Pair
    style="Rewrite Type",  # Different markers for Struct vs Vector
    data=df_filtered,
    s=100,
    edgecolor="black"
)

# Labels and title
plt.xlabel("File Size (MB)", fontsize=12)
plt.ylabel("Write Time (s)", fontsize=12)
plt.title(f"Struct vs. Vector Comparison for {large_file}", fontsize=14)
plt.grid(alpha=0.5)

# Adjust legend
plt.legend(title="Algorithm-Level Pair", fontsize=10, title_fontsize=12)

# Save and show plot
plt.tight_layout()
plt.savefig("struct_vs_vector_scatter_algo_level.png", dpi=300)
# # plt.show()



import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load dataset
file_path = "combined_results_more_variables.csv"  # Replace with actual file path
df = pd.read_csv(file_path)

# Extract base file names to match Struct and Vector versions
df["Base File Name"] = df["File Name"].str.replace(r"(_struct|_vector)\.root$", ".root", regex=True)

# Select a single large file for side-by-side comparison
large_file = "large_combined_vect_file.root"  # Adjust this if needed
df_filtered = df[df["Base File Name"] == large_file]

# Merge Struct and Vector data
df_struct = df_filtered[df_filtered["Rewrite Type"] == "Struct"]
df_vector = df_filtered[df_filtered["Rewrite Type"] == "Vector"]

# Ensure both Struct and Vector versions exist for each algorithm/compression setting
df_merged = pd.merge(
    df_struct, df_vector,
    on=["Base File Name", "Algorithm", "Compression Level"],
    suffixes=("_Struct", "_Vector")
)

# Create a new column for coloring by Algorithm-Level Pair
df_merged["Algo-Level Pair"] = df_merged["Algorithm"].astype(str) + ", " + df_merged["Compression Level"].astype(str)

# Scatter plot
plt.figure(figsize=(10, 6))

# Plot Struct-based points on the x-axis
sns.scatterplot(
    x="File Size (MB)_Struct",
    y="Write Time (s)_Struct",
    hue="Algo-Level Pair",
    data=df_merged,
    palette="tab10",
    s=120,
    linewidth=2,
    edgecolor="black",
    marker="x"  # Struct is plotted as 'X'
)

# Plot Vector-based circles at the same x-values
sns.scatterplot(
    x="File Size (MB)_Struct",
    y="Write Time (s)_Vector",
    hue="Algo-Level Pair",
    data=df_merged,
    palette="tab10",
    s=100,
    edgecolor="black",
    marker="o",  # Vector is plotted as circles
    legend=False  # Avoid duplicate legend entries
)

# Labels and title
plt.xlabel("File Size (MB) - Struct", fontsize=12)
plt.ylabel("Write Time (s)", fontsize=12)
plt.title(f"Struct (X) vs. Vector (O) Comparison for {large_file}", fontsize=14)
plt.grid(alpha=0.5)

# Adjust legend title
plt.legend(title="Algorithm-Level Pair", fontsize=10, title_fontsize=12)

# Save and show plot
plt.tight_layout()
plt.savefig("struct_vs_vector_xo_comparison.png", dpi=300)

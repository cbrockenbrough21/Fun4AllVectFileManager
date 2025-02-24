import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the normalized data
file_path = "combined_results_more_variables.csv"  # Updated CSV file path
df = pd.read_csv(file_path)

# Plot 1: Scatter plot colored by File Name (All Data)
plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="% Change Write Time",
    y="% Change File Size",
    hue="Base File Name",
    data=df,
    palette="tab10",
    s=100,
    alpha=0.8,
    edgecolor="black"
)
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)
plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)
plt.title("Vector vs Struct: % Change Colored by File Name (All Data)", fontsize=14)
plt.xlabel("% Change in Write Time (Struct vs Vector)", fontsize=12)
plt.ylabel("% Change in File Size (Struct vs Vector)", fontsize=12)
plt.legend(title="File Name", fontsize=8, title_fontsize=10, loc="best", bbox_to_anchor=(1.05, 1))
plt.grid(alpha=0.5)
plt.tight_layout()
plt.savefig("vector_vs_struct_by_file_name_all.png", dpi=300)
plt.close()

# Plot 2: Scatter plot colored by Algorithm-Level Pair (All Data)
df["Algo-Level Pair"] = df["Algorithm"].astype(str) + ", " + df["Compression Level"].astype(str)

plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="% Change Write Time",
    y="% Change File Size",
    hue="Algo-Level Pair",
    data=df,
    palette="tab10",
    s=100,
    alpha=0.8,
    edgecolor="black"
)
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)
plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)
plt.title("Vector vs Struct: % Change Colored by Algo-Level Pair (All Data)", fontsize=14)
plt.xlabel("% Change in Write Time (Struct vs Vector)", fontsize=12)
plt.ylabel("% Change in File Size (Struct vs Vector)", fontsize=12)
plt.legend(title="Algo-Level Pair", fontsize=8, title_fontsize=10, loc="best", bbox_to_anchor=(1.05, 1))
plt.grid(alpha=0.5)
plt.tight_layout()
plt.savefig("vector_vs_struct_by_algo_level_all.png", dpi=300)
plt.close()

# Filter Data for the Specific Files
specific_files = ["large_combined_vect_file.root", "large_combined_vect_file_struct.root"]
df_filtered = df[df["Base File Name"].isin(specific_files)]

# Plot 3: Scatter plot for specific file
plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="% Change Write Time",
    y="% Change File Size",
    hue="Base File Name",
    data=df_filtered,
    palette="tab10",
    s=100,
    alpha=0.8,
    edgecolor="black"
)
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)
plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)
plt.title("Vector vs Struct: % Change (Filtered: large_combined_vect_file)", fontsize=14)
plt.xlabel("% Change in Write Time (Struct vs Vector)", fontsize=12)
plt.ylabel("% Change in File Size (Struct vs Vector)", fontsize=12)
plt.legend(title="File Name", fontsize=8, title_fontsize=10, loc="best", bbox_to_anchor=(1.05, 1))
plt.grid(alpha=0.5)
plt.tight_layout()
plt.savefig("vector_vs_struct_large_combined_filtered.png", dpi=300)
plt.close()

print("Plots saved as:")
print(" - 'vector_vs_struct_by_file_name_all.png'")
print(" - 'vector_vs_struct_by_algo_level_all.png'")
print(" - 'vector_vs_struct_large_combined_filtered.png'")

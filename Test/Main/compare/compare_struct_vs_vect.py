import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re

# Load the dataset
file_path = "combined_results_more_variables.csv"  # Replace with actual file path
df = pd.read_csv(file_path)

# Ensure required columns exist
required_columns = ["File Name", "Rewrite Type", "Algorithm", "Compression Level", "File Size (MB)", "Write Time (s)"]
for col in required_columns:
    if col not in df.columns:
        raise ValueError(f"Missing expected column: {col}")

# Extract base file names by removing _struct and _vector suffixes
def extract_base_name(file_name):
    return re.sub(r"(_struct|_vector)\.root$", ".root", file_name)

df["Base File Name"] = df["File Name"].apply(extract_base_name)

# Separate struct-based and vector-based files
df_struct = df[df["Rewrite Type"] == "Struct"].copy()
df_vector = df[df["Rewrite Type"] == "Vector"].copy()

# Merge struct and vector data using Base File Name, Algorithm, and Compression Level
df_merged = pd.merge(
    df_struct, df_vector, 
    on=["Base File Name", "Algorithm", "Compression Level"],
    suffixes=("_Struct", "_Vector")
)

# Compute percentage change
df_merged["% Change Write Time"] = ((df_merged["Write Time (s)_Struct"] - df_merged["Write Time (s)_Vector"]) / df_merged["Write Time (s)_Vector"]) * 100
df_merged["% Change File Size"] = ((df_merged["File Size (MB)_Struct"] - df_merged["File Size (MB)_Vector"]) / df_merged["File Size (MB)_Vector"]) * 100

# Scatter Plot 1: Colored by File Name (All Data)
plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="% Change Write Time",
    y="% Change File Size",
    hue="Base File Name",
    data=df_merged,
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

# Scatter Plot 2: Colored by Algorithm-Level Pair (All Data)
df_merged["Algo-Level Pair"] = df_merged["Algorithm"].astype(str) + ", " + df_merged["Compression Level"].astype(str)

plt.figure(figsize=(10, 6))
sns.scatterplot(
    x="% Change Write Time",
    y="% Change File Size",
    hue="Algo-Level Pair",
    data=df_merged,
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

# Filter Data for the Specific File
specific_files = ["large_combined_vect_file.root"]
df_filtered = df_merged[df_merged["Base File Name"].isin(specific_files)]

# Scatter Plot 3: Filtered for Specific File
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

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the normalized data
file_path = "vector_vs_struct_percentage_comparison.csv"  # Replace with your CSV file path
df = pd.read_csv(file_path)

# Plot 1: Scatter plot colored by File Name
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
plt.title("Vector vs Struct: % Change Colored by File Name", fontsize=14)
plt.xlabel("% Change in Write Time (Struct vs Vector)", fontsize=12)
plt.ylabel("% Change in File Size (Struct vs Vector)", fontsize=12)
plt.legend(title="File Name", fontsize=8, title_fontsize=10, loc="best", bbox_to_anchor=(1.05, 1))
plt.grid(alpha=0.5)
plt.tight_layout()
plt.savefig("vector_vs_struct_by_file_name.png", dpi=300)
plt.close()

# Plot 2: Scatter plot colored by Algorithm-Level Pair
# Create a combined column for Algo-Level Pair
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
plt.title("Vector vs Struct: % Change Colored by Algo-Level Pair", fontsize=14)
plt.xlabel("% Change in Write Time (Struct vs Vector)", fontsize=12)
plt.ylabel("% Change in File Size (Struct vs Vector)", fontsize=12)
plt.legend(title="Algo-Level Pair", fontsize=8, title_fontsize=10, loc="best", bbox_to_anchor=(1.05, 1))
plt.grid(alpha=0.5)
plt.tight_layout()
plt.savefig("vector_vs_struct_by_algo_level.png", dpi=300)
plt.close()

print("Plots saved as 'vector_vs_struct_by_file_name.png' and 'vector_vs_struct_by_algo_level.png'.")

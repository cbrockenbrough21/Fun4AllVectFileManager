import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load datasets
old_df = pd.read_csv("combined_results_no_scale.csv")
new_df = pd.read_csv("combined_results_more_variables.csv")

# Define filtering conditions
def filter_conditions(df):
    return df[(df["Basket Size"] == 64000) & (df["AutoFlush"] == 2500)]

# Filter each DataFrame independently
old_filtered = filter_conditions(old_df)
new_filtered = filter_conditions(new_df)

# Merge on File Name, Algorithm, Compression Level
merged_df = pd.merge(old_filtered, new_filtered, on=["File Name", "Algorithm", "Compression Level"], suffixes=("_old", "_new"))

# Compute percentage increase
merged_df["Size Increase (%)"] = ((merged_df["File Size (MB)_new"] - merged_df["File Size (MB)_old"]) / merged_df["File Size (MB)_old"]) * 100
merged_df["Time Increase (%)"] = ((merged_df["Write Time (s)_new"] - merged_df["Write Time (s)_old"]) / merged_df["Write Time (s)_old"]) * 100

# Create a new column for Algorithm-Level pair
merged_df["Algo_Level"] = merged_df["Algorithm"].astype(str) + "_" + merged_df["Compression Level"].astype(str)

# --- Visualization ---
plt.figure(figsize=(12, 6))

# Scatter plot colored by File Name
plt.figure(figsize=(8, 6))
sns.scatterplot(data=merged_df, x="Size Increase (%)", y="Time Increase (%)", hue="File Name", palette="tab10", s=80, edgecolor="black")
plt.xlabel("File Size Increase (%)")
plt.ylabel("Write Time Increase (%)")
plt.title("Size vs. Write Time Increase (Colored by File Name)")
plt.legend(loc="best", fontsize=8, title="File Name", bbox_to_anchor=(1, 1))
plt.grid()

# Save the first scatter plot
plt.savefig("scatter_by_file_name.png", dpi=300, bbox_inches="tight")
plt.close()

# Scatter plot colored by Algorithm/Compression Level
plt.figure(figsize=(8, 6))
sns.scatterplot(data=merged_df, x="Size Increase (%)", y="Time Increase (%)", hue="Algo_Level", palette="Set1", s=80, edgecolor="black")
plt.xlabel("File Size Increase (%)")
plt.ylabel("Write Time Increase (%)")
plt.title("Size vs. Write Time Increase (Colored by Algorithm/Level)")
plt.legend(loc="best", fontsize=8, title="Algorithm_Level", bbox_to_anchor=(1, 1))
plt.grid()

# Save the second scatter plot
plt.savefig("scatter_by_algo_level.png", dpi=300, bbox_inches="tight")
plt.close()

print("Scatter plots saved as 'scatter_by_file_name.png' and 'scatter_by_algo_level.png'")

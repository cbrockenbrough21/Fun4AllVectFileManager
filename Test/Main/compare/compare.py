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

# Merge on File Name, Rewrite Type, Algorithm, Compression Level
merged_df = pd.merge(old_filtered, new_filtered, 
                      on=["File Name", "Rewrite Type", "Algorithm", "Compression Level"], 
                      suffixes=("_old", "_new"))

# Compute percentage increase
merged_df["Size Increase (%)"] = ((merged_df["File Size (MB)_new"] - merged_df["File Size (MB)_old"]) / merged_df["File Size (MB)_old"]) * 100
merged_df["Time Increase (%)"] = ((merged_df["Write Time (s)_new"] - merged_df["Write Time (s)_old"]) / merged_df["Write Time (s)_old"]) * 100

# Create a new column for Algorithm-Level pair
merged_df["Algo_Level"] = merged_df["Algorithm"].astype(str) + "_" + merged_df["Compression Level"].astype(str)

# Separate Struct and Vector Data
struct_df = merged_df[merged_df["Rewrite Type"] == "Struct"]
vector_df = merged_df[merged_df["Rewrite Type"] == "Vector"]

# --- Function for Plotting ---
def create_scatter_plot(df, title_suffix, filename_suffix):
    plt.figure(figsize=(8, 6))
    sns.scatterplot(data=df, x="Size Increase (%)", y="Time Increase (%)", hue="File Name", palette="tab10", s=80, edgecolor="black")
    plt.xlabel("File Size Increase (%)")
    plt.ylabel("Write Time Increase (%)")
    plt.title(f"Size vs. Write Time Increase ({title_suffix} - Colored by File Name)")
    plt.legend(loc="best", fontsize=8, title="File Name", bbox_to_anchor=(1, 1))
    plt.grid()
    plt.savefig(f"scatter_by_file_name_{filename_suffix}.png", dpi=300, bbox_inches="tight")
    plt.close()

    # Scatter plot colored by Algorithm/Compression Level
    plt.figure(figsize=(8, 6))
    sns.scatterplot(data=df, x="Size Increase (%)", y="Time Increase (%)", hue="Algo_Level", palette="Set1", s=80, edgecolor="black")
    plt.xlabel("File Size Increase (%)")
    plt.ylabel("Write Time Increase (%)")
    plt.title(f"Size vs. Write Time Increase ({title_suffix} - Colored by Algorithm/Level)")
    plt.legend(loc="best", fontsize=8, title="Algorithm_Level", bbox_to_anchor=(1, 1))
    plt.grid()
    plt.savefig(f"scatter_by_algo_level_{filename_suffix}.png", dpi=300, bbox_inches="tight")
    plt.close()

# Generate plots separately for Struct and Vector
create_scatter_plot(struct_df, "Struct", "struct")
create_scatter_plot(vector_df, "Vector", "vector")

print("Scatter plots saved separately for Struct and Vector.")

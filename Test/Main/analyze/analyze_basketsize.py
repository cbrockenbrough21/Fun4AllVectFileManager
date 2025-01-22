import pandas as pd
import matplotlib.pyplot as plt

# Load the combined results CSV
df = pd.read_csv("combined_results.csv")  # Replace with your file path

# Group data by parameters excluding Basket Size
grouped = df.groupby(
    ["Rewrite Type", "Algorithm", "Compression Level", "AutoFlush", "File Name"]
)

# Separate basket size cases and reset the index
basket_32000 = grouped.apply(lambda x: x[x["Basket Size"] == 32000]).reset_index(drop=True)
basket_64000 = grouped.apply(lambda x: x[x["Basket Size"] == 64000]).reset_index(drop=True)

# Merge the two datasets for comparison
comparison_df = basket_32000.merge(
    basket_64000,
    on=["Rewrite Type", "Algorithm", "Compression Level", "AutoFlush", "File Name"],
    suffixes=("_32000", "_64000"),
    how="inner"  # Only include cases where both basket sizes exist
)

# Determine which basket size performs better
comparison_df["Better_Write_Time"] = comparison_df["Write Time (s)_64000"] < comparison_df["Write Time (s)_32000"]
comparison_df["Better_File_Size"] = comparison_df["File Size (MB)_64000"] < comparison_df["File Size (MB)_32000"]

# Add an Algo/Level column
comparison_df["Algo/Level"] = comparison_df["Algorithm"].astype(str) + "/" + comparison_df["Compression Level"].astype(str)

# Count occurrences for each Algo/Level pair
summary = comparison_df.groupby("Algo/Level").agg(
    Better_Write_Time_64000=("Better_Write_Time", "sum"),  # Count where 64000 performed better
    Better_File_Size_64000=("Better_File_Size", "sum"),   # Count where 64000 performed better
    Total=("Better_Write_Time", "size")                  # Total cases for the pair
).reset_index()

# Calculate counts for 32000 performing better
summary["Better_Write_Time_32000"] = summary["Total"] - summary["Better_Write_Time_64000"]
summary["Better_File_Size_32000"] = summary["Total"] - summary["Better_File_Size_64000"]

# Visualize the results
plt.figure(figsize=(12, 6))
bar_width = 0.4
x = range(len(summary))

# Plot write time comparison
plt.bar([pos - bar_width / 2 for pos in x], summary["Better_Write_Time_64000"], bar_width, label="Write Time (64000)", color="blue")
plt.bar([pos - bar_width / 2 for pos in x], summary["Better_Write_Time_32000"], bar_width, bottom=summary["Better_Write_Time_64000"], label="Write Time (32000)", color="lightblue")

# Plot file size comparison
plt.bar([pos + bar_width / 2 for pos in x], summary["Better_File_Size_64000"], bar_width, label="File Size (64000)", color="green")
plt.bar([pos + bar_width / 2 for pos in x], summary["Better_File_Size_32000"], bar_width, bottom=summary["Better_File_Size_64000"], label="File Size (32000)", color="lightgreen")

# Add labels and legend
plt.xticks(x, summary["Algo/Level"], rotation=45, ha="right", fontsize=10)
plt.title("Comparison of Basket Sizes (32000 vs. 64000) by Algo/Level Pair", fontsize=14)
plt.xlabel("Algo/Level Pair", fontsize=12)
plt.ylabel("Count of Better Performances", fontsize=12)
plt.legend(title="Metric and Basket Size", fontsize=10, title_fontsize=12)
plt.grid(axis="y", linestyle="--", alpha=0.7)
plt.tight_layout()

# Save and show the plot
plt.savefig("basket_size_comparison.png")
plt.show()

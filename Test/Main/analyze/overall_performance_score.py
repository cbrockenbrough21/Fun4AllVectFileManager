import pandas as pd
import matplotlib.pyplot as plt

# Load the combined results CSV
df = pd.read_csv("combined_results.csv")

# Normalize Write Time and File Size for each file individually
def normalize_by_file(group):
    group["Normalized Write Time"] = group["Write Time (s)"] / group["Write Time (s)"].max()
    group["Normalized File Size"] = group["File Size (MB)"] / group["File Size (MB)"].max()
    return group

df = df.groupby("File Name").apply(normalize_by_file)

# Weights for metrics
w1 = 0.65  # Weight for Write Time
w2 = 0.35  # Weight for File Size

# Calculate Performance Score for each row
df["Performance Score"] = (
    w1 * df["Normalized Write Time"] + w2 * df["Normalized File Size"]
)

# Find the best combination for each file
best_per_file = (
    df.groupby("File Name")
    .apply(lambda x: x.loc[x["Performance Score"].idxmin()])
    .reset_index(drop=True)
)

# Save the best combination per file to a CSV
best_per_file.to_csv("best_performance_per_file.csv", index=False)

# Aggregate across files to find the average performance score for each combination
grouped_avg = (
    df.groupby(["Algorithm", "Compression Level", "Basket Size", "AutoFlush"])
    .mean(numeric_only=True)  # Average across files
    .reset_index()
)

# Sort by Performance Score (lower is better)
sorted_avg_results = grouped_avg.sort_values(by="Performance Score")

# Save the averaged results to a new CSV file
sorted_avg_results.to_csv("average_performance_scores.csv", index=False)

# Visualize results for each file and the averaged results
# plt.figure(figsize=(12, 6))

# # Plot individual best combinations for each file
# for file_name in best_per_file["File Name"].unique():
#     subset = best_per_file[best_per_file["File Name"] == file_name]
#     plt.scatter(
#         subset["Compression Level"],
#         subset["Performance Score"],
#         label=f"File: {file_name}",
#         s=100,
#         alpha=0.7,
#     )

# # Plot the averaged performance scores
# plt.plot(
#     sorted_avg_results["Compression Level"],
#     sorted_avg_results["Performance Score"],
#     marker="o",
#     label="Averaged Across Files",
#     color="black",
#     linestyle="--",
# )

# # Add plot details
# plt.title("Performance Score: Individual Files and Averages", fontsize=14)
# plt.xlabel("Compression Level", fontsize=12)
# plt.ylabel("Performance Score (Lower is Better)", fontsize=12)
# plt.legend(title="Files", fontsize=10, title_fontsize=12)
# plt.grid(True)

# # Save the plot to a file
# plt.savefig("performance_score_comparison.png", dpi=300)
# print("Plot saved as performance_score_comparison.png")

# # plt.show()

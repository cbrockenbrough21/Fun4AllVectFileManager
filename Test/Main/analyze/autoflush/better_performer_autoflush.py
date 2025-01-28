# import pandas as pd
# import matplotlib.pyplot as plt
# import seaborn as sns

# # Load the data
# file_path = "Vector_autoflush_relative_changes.csv"  # Replace with your file path
# df_relative = pd.read_csv(file_path)

# # Group by Algorithm, Compression Level, and File Name
# comparison_results = []

# for (algo, level, file_name), group in df_relative.groupby(["Algorithm", "Compression Level", "File Name"]):
#     # Filter AutoFlush=2500 and AutoFlush=5000
#     af_2500 = group[group["AutoFlush"] == 2500]
#     af_5000 = group[group["AutoFlush"] == 5000]
    
#     if not af_2500.empty and not af_5000.empty:
#         # Compare % Change Write Time
#         if af_2500["% Change Write Time"].iloc[0] < af_5000["% Change Write Time"].iloc[0]:
#             better_write_time = "2500"
#         elif af_2500["% Change Write Time"].iloc[0] > af_5000["% Change Write Time"].iloc[0]:
#             better_write_time = "5000"
#         else:
#             better_write_time = "Tie"

#         # Compare % Change File Size
#         if af_2500["% Change File Size"].iloc[0] < af_5000["% Change File Size"].iloc[0]:
#             better_file_size = "2500"
#         elif af_2500["% Change File Size"].iloc[0] > af_5000["% Change File Size"].iloc[0]:
#             better_file_size = "5000"
#         else:
#             better_file_size = "Tie"

#         comparison_results.append({
#             "Algorithm": algo,
#             "Compression Level": level,
#             "File Name": file_name,
#             "Better Write Time": better_write_time,
#             "Better File Size": better_file_size,
#         })

# # Convert results to DataFrame
# comparison_df = pd.DataFrame(comparison_results)

# # Count occurrences for each Algorithm/Level pair
# write_time_counts = comparison_df.groupby(["Algorithm", "Compression Level"])["Better Write Time"].value_counts().unstack(fill_value=0)
# file_size_counts = comparison_df.groupby(["Algorithm", "Compression Level"])["Better File Size"].value_counts().unstack(fill_value=0)

# # Plotting for Write Time
# plt.figure(figsize=(12, 6))
# write_time_counts.plot(
#     kind="bar",
#     stacked=True,
#     colormap="coolwarm",
#     alpha=0.8,
#     figsize=(12, 6)
# )
# plt.title("AutoFlush Performance: Write Time Comparison", fontsize=14)
# plt.xlabel("Algorithm/Level Pair", fontsize=12)
# plt.ylabel("Count", fontsize=12)
# plt.xticks(rotation=45)
# plt.legend(title="Better AutoFlush")
# plt.tight_layout()
# plt.savefig("autoflush_write_time_comparison.png", dpi=300)

# # Plotting for File Size
# plt.figure(figsize=(12, 6))
# file_size_counts.plot(
#     kind="bar",
#     stacked=True,
#     colormap="coolwarm",
#     alpha=0.8,
#     figsize=(12, 6)
# )
# plt.title("AutoFlush Performance: File Size Comparison", fontsize=14)
# plt.xlabel("Algorithm/Level Pair", fontsize=12)
# plt.ylabel("Count", fontsize=12)
# plt.xticks(rotation=45)
# plt.legend(title="Better AutoFlush")
# plt.tight_layout()
# plt.savefig("autoflush_file_size_comparison.png", dpi=300)


import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the data
file_path = "Vector_autoflush_relative_changes.csv"  # Replace with your file path
df_relative = pd.read_csv(file_path)

# Group by Algorithm, Compression Level, and File Name
comparison_results = []

for (algo, level, file_name), group in df_relative.groupby(["Algorithm", "Compression Level", "File Name"]):
    # Filter AutoFlush=2500 and AutoFlush=5000
    af_2500 = group[group["AutoFlush"] == 2500]
    af_5000 = group[group["AutoFlush"] == 5000]
    
    if not af_2500.empty and not af_5000.empty:
        # Compare % Change Write Time
        if af_2500["% Change Write Time"].iloc[0] < af_5000["% Change Write Time"].iloc[0]:
            better_write_time = "2500"
        elif af_2500["% Change Write Time"].iloc[0] > af_5000["% Change Write Time"].iloc[0]:
            better_write_time = "5000"
        else:
            better_write_time = "Tie"

        # Compare % Change File Size
        if af_2500["% Change File Size"].iloc[0] < af_5000["% Change File Size"].iloc[0]:
            better_file_size = "2500"
        elif af_2500["% Change File Size"].iloc[0] > af_5000["% Change File Size"].iloc[0]:
            better_file_size = "5000"
        else:
            better_file_size = "Tie"

        comparison_results.append({
            "Algorithm": algo,
            "Compression Level": level,
            "File Name": file_name,
            "Better Write Time": better_write_time,
            "Better File Size": better_file_size,
        })

# Convert results to DataFrame
comparison_df = pd.DataFrame(comparison_results)

# Count occurrences for each Algorithm/Level pair
write_time_counts = comparison_df.groupby(["Algorithm", "Compression Level"])["Better Write Time"].value_counts().unstack(fill_value=0)
file_size_counts = comparison_df.groupby(["Algorithm", "Compression Level"])["Better File Size"].value_counts().unstack(fill_value=0)

# Combine Write Time and File Size counts
combined_counts = pd.concat([write_time_counts, file_size_counts], keys=["Write Time", "File Size"])
combined_counts = combined_counts.fillna(0)

# Create a combined bar chart
categories = combined_counts.index.get_level_values(1).unique()  # Autoflush categories
bar_width = 0.35  # Width of the bars
x_indices = np.arange(len(combined_counts.index.levels[0]))  # Algorithm/Level pairs

plt.figure(figsize=(14, 8))

# Plot Write Time bars
for i, autoflush in enumerate(categories):
    plt.bar(
        x_indices - bar_width / 2 + i * bar_width / len(categories),
        write_time_counts[autoflush],
        bar_width / len(categories),
        label=f"Write Time AF{autoflush}",
        alpha=0.7
    )

# Plot File Size bars
for i, autoflush in enumerate(categories):
    plt.bar(
        x_indices + bar_width / 2 + i * bar_width / len(categories),
        file_size_counts[autoflush],
        bar_width / len(categories),
        label=f"File Size AF{autoflush}",
        alpha=0.7
    )

# Add plot details
plt.title("AutoFlush Performance: Write Time and File Size Comparison", fontsize=16)
plt.xlabel("Algorithm/Level Pair", fontsize=14)
plt.ylabel("Count", fontsize=14)
plt.xticks(x_indices, [f"Algo {a}, Level {l}" for a, l in combined_counts.index.levels[0]], rotation=45)
plt.legend(title="AutoFlush Settings", fontsize=12)
plt.grid(True, axis="y", linestyle="--", alpha=0.6)
plt.tight_layout()

# Save the plot
plt.savefig("autoflush_combined_comparison.png", dpi=300)
plt.show()

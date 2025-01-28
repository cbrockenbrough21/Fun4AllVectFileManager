
import pandas as pd
import matplotlib.pyplot as plt

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

# Combine the data for Write Time and File Size
combined_counts = pd.concat(
    {"Write Time": write_time_counts, "File Size": file_size_counts},
    axis=1
)

# Define custom colors for the bar charts
custom_colors = {"2500": "pink", "5000": "blue", "Tie": "gray"}

# Flatten MultiIndex for better plotting
combined_counts.columns = ["_".join(col).strip() for col in combined_counts.columns]

# Plotting
combined_counts.plot(
    kind="bar",
    color=[custom_colors.get(col.split("_")[1], "gray") for col in combined_counts.columns],
    alpha=0.8,
    figsize=(12, 6)
)

# Add labels and title
plt.title("AutoFlush Performance: Write Time and File Size Comparison", fontsize=14)
plt.xlabel("Algorithm/Level Pair", fontsize=12)
plt.ylabel("Count", fontsize=12)
plt.xticks(rotation=45)
plt.legend(title="Metric_AutoFlush")
plt.tight_layout()
plt.savefig("autoflush_combined_comparison.png", dpi=300)
plt.show()

# import pandas as pd
# import matplotlib.pyplot as plt

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

# # Define custom colors for the bar charts
# custom_colors = {"2500": "pink", "5000": "blue", "Tie": "gray"}

# # Plotting for Write Time and File Size in the same figure
# fig, axes = plt.subplots(1, 2, figsize=(16, 6), sharey=True)

# # Write Time Plot
# write_time_counts.plot(
#     kind="bar",
#     stacked=True,
#     color=[custom_colors.get(col, "gray") for col in write_time_counts.columns],
#     alpha=0.8,
#     ax=axes[0]
# )
# axes[0].set_title("AutoFlush Performance: Write Time Comparison", fontsize=14)
# axes[0].set_xlabel("Algorithm/Level Pair", fontsize=12)
# axes[0].set_ylabel("Count", fontsize=12)
# axes[0].tick_params(axis='x', rotation=45)
# axes[0].legend(title="Better AutoFlush")

# # File Size Plot
# file_size_counts.plot(
#     kind="bar",
#     stacked=True,
#     color=[custom_colors.get(col, "gray") for col in file_size_counts.columns],
#     alpha=0.8,
#     ax=axes[1]
# )
# axes[1].set_title("AutoFlush Performance: File Size Comparison", fontsize=14)
# axes[1].set_xlabel("Algorithm/Level Pair", fontsize=12)
# axes[1].tick_params(axis='x', rotation=45)
# axes[1].legend(title="Better AutoFlush")

# plt.tight_layout()
# plt.savefig("autoflush_comparison_combined.png", dpi=300)

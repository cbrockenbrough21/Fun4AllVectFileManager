import pandas as pd

# Load the combined results CSV
file_path = "combined_results.csv"  # Replace with your file path
df = pd.read_csv(file_path)

# Filter for a specific rewrite type if needed (e.g., "Vector" or "Struct")
rewrite_type = "Vector"  # Change to "Struct" for struct analysis
filtered_df = df[df["Rewrite Type"] == rewrite_type]

# Function to calculate percentage change relative to autoflush 0 for each file
def calculate_relative_change(group):
    # Find baseline where AutoFlush == 0
    baseline = group[group["AutoFlush"] == 0]
    if not baseline.empty:
        # Get baseline values (assumes one row for AutoFlush == 0 per group)
        baseline_values = baseline[["Write Time (s)", "File Size (MB)"]].iloc[0]
        group["% Change Write Time"] = (
            (group["Write Time (s)"] - baseline_values["Write Time (s)"]) /
            baseline_values["Write Time (s)"]
        ) * 100
        group["% Change File Size"] = (
            (group["File Size (MB)"] - baseline_values["File Size (MB)"]) /
            baseline_values["File Size (MB)"]
        ) * 100
    else:
        group["% Change Write Time"] = None
        group["% Change File Size"] = None
    return group

# Apply the function to calculate percentage changes for each file and configuration
df_relative = filtered_df.groupby(
    ["File Name", "Algorithm", "Compression Level", "Basket Size"], group_keys=False
).apply(calculate_relative_change)

# Save the new CSV file with relative percentage changes
output_path = f"{rewrite_type}_autoflush_relative_changes.csv"  # Dynamic filename based on rewrite type
df_relative.to_csv(output_path, index=False)
print(f"Relative changes saved to {output_path}")

import pandas as pd
import matplotlib.pyplot as plt

# Load the combined CSV file
file_path = "../combined_results.csv"  # Replace with your combined CSV file path
df = pd.read_csv(file_path)

# Filter for Algo 2, Level 3
df = df[(df["Algorithm"] == 2) & (df["Compression Level"] == 3)]

# Filter for the baseline (Algo 2, Level 3, Basket Size 32000, AutoFlush 0)
baseline = df[
    (df["Basket Size"] == 32000) & (df["AutoFlush"] == 0)
]

# Check for duplicates in baseline
duplicates = baseline[baseline.duplicated(subset="File Name", keep=False)]
if not duplicates.empty:
    print("Duplicates found in baseline:")
    print(duplicates)
    exit()

# Create a dictionary for baseline values (ensures one baseline per file)
baseline_dict = baseline.set_index("File Name")[["Write Time (s)", "File Size (MB)"]].to_dict("index")

# Add % Change Columns
def calculate_percentage_change(row, baseline_dict):
    file_name = row["File Name"]
    baseline = baseline_dict.get(file_name, None)
    if not baseline:
        return pd.Series([None, None])  # No baseline, return None
    file_size_change = ((row["File Size (MB)"] - baseline["File Size (MB)"]) / baseline["File Size (MB)"]) * 100
    write_time_change = ((row["Write Time (s)"] - baseline["Write Time (s)"]) / baseline["Write Time (s)"]) * 100
    return pd.Series([file_size_change, write_time_change])

df[["%_Change_File_Size", "%_Change_Write_Time"]] = df.apply(
    calculate_percentage_change, axis=1, baseline_dict=baseline_dict
)

# Save the updated DataFrame for debugging or further analysis
df.to_csv("algo2_level3_results_with_percentage_changes.csv", index=False)

# Define color mapping for AutoFlush and marker mapping for Basket Size
autoflush_colors = {0: "blue", 2500: "orange", 5000: "green"}
basket_markers = {32000: "o", 64000: "s"}

# Scatter Plot for % Change in Write Time vs File Size
plt.figure(figsize=(10, 6))

for autoflush, autoflush_group in df.groupby("AutoFlush"):
    for basket_size, basket_group in autoflush_group.groupby("Basket Size"):
        plt.scatter(
            basket_group["%_Change_Write_Time"],
            basket_group["%_Change_File_Size"],
            label=f"AutoFlush {autoflush}, Basket {basket_size}",
            color=autoflush_colors[autoflush],
            marker=basket_markers[basket_size],
            s=100,  # Marker size
            alpha=0.8
        )

# Plot Details
plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)  # Horizontal reference line
plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)  # Vertical reference line
plt.title("Algo 2 Level 3: % Change in Write Time vs File Size", fontsize=14)
plt.xlabel("% Change in Write Time", fontsize=12)
plt.ylabel("% Change in File Size", fontsize=12)
plt.legend(title="Configurations", fontsize=10, title_fontsize=12, loc="best")
plt.grid(alpha=0.5)
plt.tight_layout()

# Save and Show Plot
plt.savefig("algo2_level3_tradeoffs.png", dpi=300)
plt.show()

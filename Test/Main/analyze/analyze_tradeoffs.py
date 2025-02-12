import pandas as pd
import matplotlib.pyplot as plt

# Load the combined CSV file
file_path = "combined_results.csv"  # Replace with your combined CSV file path
df = pd.read_csv(file_path)

# Filter for the baseline (algo 1, level 5, basket size 32000, autoflush 0)
baseline = df[
    (df["Algorithm"] == 1) &
    (df["Compression Level"] == 5) &
    (df["Basket Size"] == 32000) &
    (df["AutoFlush"] == 0)
]

# Ensure there is only one baseline per file
baseline_dict = baseline.set_index("File Name")[["Write Time (s)", "File Size (MB)"]].to_dict("index")

# Add % change columns to the DataFrame
def calculate_percentage_change(row, baseline_dict):
    file_name = row["File Name"]
    baseline = baseline_dict.get(file_name, None)
    if not baseline:
        return pd.Series([None, None])  # If no baseline, return None
    
    file_size_change = ((row["File Size (MB)"] - baseline["File Size (MB)"]) / baseline["File Size (MB)"]) * 100
    write_time_change = ((row["Write Time (s)"] - baseline["Write Time (s)"]) / baseline["Write Time (s)"]) * 100
    return pd.Series([file_size_change, write_time_change])

df[["%_Change_File_Size", "%_Change_Write_Time"]] = df.apply(
    calculate_percentage_change, axis=1, baseline_dict=baseline_dict
)

# Save the updated DataFrame with % changes to a new CSV
df.to_csv("combined_results_with_percentage_changes.csv", index=False)

# Define a color map for algo-level pairs
color_map = {
    (1, 5): "green",
    (1, 6): "blue",
    (1, 7): "purple",
    (2, 3): "yellow",
    (2, 4): "orange",
    (2, 5): "red",
}

# Define markers for file types
file_markers = {
    "Mega File": "*",  # Star for mega
    "Large File": "D"  # Diamond for large
}

# Function to determine file marker
def assign_marker(file_name):
    if "mega" in file_name.lower():
        return "*"
    elif "large" in file_name.lower():
        return "D"
    else:
        return "o"

# Define a function to generate scatter plots
def plot_tradeoffs(rewrite_type, data, output_filename):
    # Filter for the specified rewrite type
    filtered_data = data[data["Rewrite Type"] == rewrite_type].copy()
    
    # Assign colors based on algo-level pair
    filtered_data["Color"] = filtered_data.apply(
        lambda row: color_map.get((row["Algorithm"], row["Compression Level"]), "gray"),
        axis=1
    )

    # Assign markers based on file type
    filtered_data["Marker"] = filtered_data["File Name"].apply(assign_marker)

    plt.figure(figsize=(10, 6))
    
    # Plot data
    for (algo, level), color in color_map.items():
        subset = filtered_data[(filtered_data["Algorithm"] == algo) & (filtered_data["Compression Level"] == level)]
        for marker in subset["Marker"].unique():
            marker_subset = subset[subset["Marker"] == marker]
            plt.scatter(
                marker_subset["%_Change_Write_Time"],
                marker_subset["%_Change_File_Size"],
                color=color,
                alpha=0.8,
                edgecolor="black",
                s=50,
                marker=marker
            )

    # Add plot details
    plt.title(f"Trade-offs: % Change in Write Time vs File Size ({rewrite_type} Rewrite)", fontsize=14)
    plt.xlabel("% Change in Write Time", fontsize=12)
    plt.ylabel("% Change in File Size", fontsize=12)
    plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)  # Add horizontal reference line
    plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)  # Add vertical reference line
    plt.grid(alpha=0.5)

    # **Legend: Colors for algorithm-level pairs (circle markers)**
    handles = [plt.Line2D([0], [0], marker='o', color='w', markerfacecolor=color, markersize=8, label=f"Algo {algo}, Level {level}") for (algo, level), color in color_map.items()]
    
    # **Legend: Shapes for file types**
    shape_handles = [
        plt.Line2D([0], [0], marker='*', color='w', markerfacecolor="black", markersize=10, label="Mega File"),
        plt.Line2D([0], [0], marker='D', color='w', markerfacecolor="black", markersize=8, label="Large File")
    ]
    
    # Combine legends and place in upper right corner
    plt.legend(handles=handles + shape_handles, title="Algo-Level Pairs & File Type", fontsize=10, title_fontsize=12, loc="upper right")

    plt.tight_layout()

    # Save the plot
    plt.savefig(output_filename)
    plt.close()  # Close the plot to avoid overlap in subsequent plots

# Generate plots for Vector and Struct rewrite types
plot_tradeoffs("Vector", df, "scatter_plot_tradeoffs_vector_colored.png")
plot_tradeoffs("Struct", df, "scatter_plot_tradeoffs_struct_colored.png")

print("Plots saved as 'scatter_plot_tradeoffs_vector_colored.png' and 'scatter_plot_tradeoffs_struct_colored.png'.")

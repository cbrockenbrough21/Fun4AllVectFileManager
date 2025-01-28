import pandas as pd
import matplotlib.pyplot as plt

# Load the combined CSV file
file_path = "combined_results_with_percentage_changes.csv"  # Replace with your file path
df = pd.read_csv(file_path)

# Define a color map for algo-level pairs
color_map = {
    (1, 5): "green",
    (1, 6): "blue",
    (1, 7): "purple",
    (2, 3): "yellow",
    (2, 4): "orange",
    (2, 5): "red",
}

# Define a function to generate scatter plots
def plot_tradeoffs(rewrite_type, data, output_filename):
    # Filter for the specified rewrite type
    filtered_data = data[data["Rewrite Type"] == rewrite_type].copy()
    
    # Assign colors to each row based on algo-level pair
    filtered_data["Color"] = filtered_data.apply(
        lambda row: color_map.get((row["Algorithm"], row["Compression Level"]), "gray"),
        axis=1
    )

    # Create scatter plot
    plt.figure(figsize=(10, 6))
    for (algo, level), color in color_map.items():
        subset = filtered_data[(filtered_data["Algorithm"] == algo) & (filtered_data["Compression Level"] == level)]
        plt.scatter(
            subset["%_Change_Write_Time"],
            subset["%_Change_File_Size"],
            label=f"Algo {algo}, Level {level}",
            color=color,
            alpha=0.8,
            edgecolor="black",  # Add edge for better visibility
            s=50  # Adjust marker size
        )

    # Add plot details
    plt.title(f"Trade-offs: % Change in Write Time vs File Size ({rewrite_type} Rewrite)", fontsize=14)
    plt.xlabel("% Change in Write Time", fontsize=12)
    plt.ylabel("% Change in File Size", fontsize=12)
    plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)  # Add horizontal reference line
    plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)  # Add vertical reference line
    plt.grid(alpha=0.5)
    plt.legend(title="Algo-Level Pairs", fontsize=10, title_fontsize=12, loc="best")
    plt.tight_layout()

    # Save the plot
    plt.savefig(output_filename)
    plt.close()  # Close the plot to avoid overlap in subsequent plots

# Generate plots for Vector and Struct rewrite types
plot_tradeoffs("Vector", df, "scatter_plot_tradeoffs_vector_colored.png")
plot_tradeoffs("Struct", df, "scatter_plot_tradeoffs_struct_colored.png")

print("Plots saved as 'scatter_plot_tradeoffs_vector_colored.png' and 'scatter_plot_tradeoffs_struct_colored.png'.")

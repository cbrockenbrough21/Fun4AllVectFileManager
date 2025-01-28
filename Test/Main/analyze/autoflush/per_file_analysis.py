import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the data
file_path = "Vector_autoflush_relative_changes.csv"  # Replace with your file path
df_relative = pd.read_csv(file_path)

# Visualization: Scatter Plot
# Define a color map for AutoFlush settings
color_map = {
    0: "blue",
    2500: "orange",
    5000: "green"
}

# Create a scatter plot
plt.figure(figsize=(10, 6))

# Create a scatter plot for each file
for file_name, group in df_relative.groupby("File Name"):
    plt.figure(figsize=(10, 6))
    for autoflush, subgroup in group.groupby("AutoFlush"):
        plt.scatter(
            subgroup["% Change Write Time"],
            subgroup["% Change File Size"],
            label=f"AutoFlush {autoflush}",
            color=color_map.get(autoflush, "gray"),
            alpha=0.8,
            s=100
        )
    # Add plot details
    plt.title(f"Effect of AutoFlush on Write Time and File Size for {file_name}", fontsize=14)
    plt.xlabel("% Change in Write Time", fontsize=12)
    plt.ylabel("% Change in File Size", fontsize=12)
    plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)
    plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)
    plt.legend(title="AutoFlush Setting", fontsize=10, title_fontsize=12, loc="best")
    plt.grid(True)
    plt.tight_layout()
    # Save plot for each file
    plt.savefig(f"scatter_plot_autoflush_{file_name.replace('.root', '')}.png", dpi=300)

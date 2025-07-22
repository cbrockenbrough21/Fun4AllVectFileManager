import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV with % changes already computed
df = pd.read_csv("combined_results_with_percentage_changes.csv")

# Define recommended configs
recommendations = {
    "Analysis (Algo 1, L5)": {
        "Algorithm": 1,
        "Compression Level": 5,
        "Basket Size": 64000,
        "AutoFlush": 2500,
        "Color": "green",
        "Marker": "o"
    },
    "Storage (Algo 2, L3)": {
        "Algorithm": 2,
        "Compression Level": 3,
        "Basket Size": 64000,
        "AutoFlush": 2500,
        "Color": "blue",
        "Marker": "s"
    }
}

# Function to plot scatter for one rewrite type
def plot_recommendations_scatter(data, rewrite_type, output_file):
    plt.figure(figsize=(10, 6))
    
    for label, config in recommendations.items():
        subset = data[
            (data["Rewrite Type"] == rewrite_type) &
            (data["Algorithm"] == config["Algorithm"]) &
            (data["Compression Level"] == config["Compression Level"]) &
            (data["Basket Size"] == config["Basket Size"]) &
            (data["AutoFlush"] == config["AutoFlush"])
        ]

        plt.scatter(
            subset["%_Change_Write_Time"],
            subset["%_Change_File_Size"],
            color=config["Color"],
            label=label,
            alpha=0.8,
            edgecolor='black',
            s=60,
            marker=config["Marker"]
        )

    # Plot settings
    plt.axhline(0, color="gray", linestyle="--", linewidth=0.8)
    plt.axvline(0, color="gray", linestyle="--", linewidth=0.8)
    plt.title(f"{rewrite_type} Rewrite: Recommended Configurations vs Baseline", fontsize=14)
    plt.xlabel("% Change in Write Time", fontsize=12)
    plt.ylabel("% Change in File Size", fontsize=12)
    plt.legend(fontsize=10, title="Configuration", title_fontsize=12, loc="best")
    plt.grid(alpha=0.4)
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()

# Create plots for both rewrite types
plot_recommendations_scatter(df, "Vector", "recommended_configs_vector.png")
plot_recommendations_scatter(df, "Struct", "recommended_configs_struct.png")

print("Saved plots: recommended_configs_vector.png and recommended_configs_struct.png")

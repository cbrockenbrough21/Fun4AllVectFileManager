import pandas as pd
import matplotlib.pyplot as plt

# Load the combined results CSV
df = pd.read_csv("combined_results.csv")

# Normalize Write Time and File Size
df["Normalized Write Time"] = df["Write Time (s)"] / df["Write Time (s)"].max()
df["Normalized File Size"] = df["File Size (MB)"] / df["File Size (MB)"].max()

# Define a range of weights for write time
weights = [i / 100 for i in range(0, 101, 5)]  # From 0 to 1 in steps of 0.05
results = []

# Calculate performance scores for each weight
for w1 in weights:
    w2 = 1 - w1
    df["Performance Score"] = (
        w1 * df["Normalized Write Time"] + w2 * df["Normalized File Size"]
    )
    # Find the best configuration for each file
    best_configs = df.groupby("File Name").apply(lambda x: x.nsmallest(1, "Performance Score"))
    best_algo_level = best_configs.groupby(["Algorithm", "Compression Level"]).size()
    results.append((w1, best_algo_level))

# Convert results into a DataFrame
results_df = pd.DataFrame({
    "Weight Write Time": [r[0] for r in results],
    "1/5": [r[1].get((1, 5), 0) for r in results],
    "2/3": [r[1].get((2, 3), 0) for r in results]
})

# Plot the results
plt.figure(figsize=(10, 6))
plt.plot(results_df["Weight Write Time"], results_df["1/5"], label="Algo 1, Level 5", marker="o")
plt.plot(results_df["Weight Write Time"], results_df["2/3"], label="Algo 2, Level 3", marker="o")
plt.title("Crossover Analysis: Weight Write Time vs Best Configuration")
plt.xlabel("Weight for Write Time")
plt.ylabel("Number of Files Best Configuration")
plt.axvline(0.6, color="gray", linestyle="--", label="Crossover Point")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("crossover_analysis.png")
# plt.show()

import pandas as pd
import matplotlib.pyplot as plt
from adjustText import adjust_text
import matplotlib.cm as cm

folder = "experiments/balls_into_bins/"

# Load the data
file_path = folder + "data.csv"  # Update with the path to your data file
data = pd.read_csv(file_path)
factor = 1000  # / 100000 (num_elements) * 100 (percentage)

# Group the data by load_factor
grouped = data.groupby("load_factor")

# Plot the data
plt.figure(figsize=(10, 6))

# Collect text objects for label adjustments
texts = []

# List of unique bin_capacity values (used for x-ticks)
unique_bin_capacities = sorted(data['bin_capacity'].unique())

# Generate evenly spaced positions for x-ticks
num_bins = len(unique_bin_capacities)
evenly_spaced_x = range(num_bins)

# Define a colormap (you can change this to a different one if preferred)
colors = cm.tab10.colors  # Using the default color palette from matplotlib

# Plot the data with evenly spaced x-tick positions
lines = []  # To store the line objects for the legend
labels = []  # To store the labels for the legend
for idx, (load_factor, group) in enumerate(grouped):
    bin_capacity = group["bin_capacity"]
    average = group["average"]
    stddev = group["stddev"]

    # Assign a unique color for each load_factor
    color = colors[idx % len(colors)]  # Cycle through the colormap if there are more than 10 load factors

    # Plot with smaller data points and visible lines with different colors
    line = plt.errorbar(evenly_spaced_x, average / factor, yerr=stddev / factor, fmt='-o', capsize=5, 
                           label=f'Load Factor {load_factor}', markersize=4, linestyle='-', color=color)  # Use unique color

    # Store the line object and label for the legend
    lines.append(line)
    labels.append(f'Load Factor {load_factor}')

    # Add labels for each data point, adjusted to the top-right
    for x, y in zip(evenly_spaced_x, average / factor):
        label = f"{y:.2f}"
        text = plt.text(x + 0.1, y + 0.05, label, fontsize=8, ha='left', va='top')  # Slight offset for top-right positioning
        texts.append(text)

# Adjust the labels to avoid overlap
adjust_text(texts, arrowprops=dict(arrowstyle="->", color='gray', lw=0.5))

# Set the x-ticks to be the evenly spaced positions and their corresponding bin_capacity labels
plt.xticks(evenly_spaced_x, unique_bin_capacities)

# Customize the plot
plt.xlabel("Bin Capacity", fontsize = 12)
plt.ylabel("Percentage of Overflowing Elements", fontsize = 12)

# Apply grid only for the y-axis
plt.grid(linestyle='--', alpha=0.7)

# Reverse the legend order by passing reversed handles and labels
plt.legend(handles=lines[::-1], labels=labels[::-1], title="Load Factor")

# Save and show the plot with the filename "plot.png" in the specified folder
plt.savefig(folder + "plot.png", dpi=300)
plt.show()

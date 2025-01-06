import pandas as pd
import matplotlib.pyplot as plt

folder = "experiments/queue_cdm_cuckoo_tables_size/"
bin_capacity = 8

# Read the CSV file
file_path = folder + "data/data_bin_cap_{}.csv".format(bin_capacity)
data = pd.read_csv(file_path)

# Group by 'insert_loop_iterations' and 'time_step', calculate the mean and std of 'queue_size'
grouped_data = data.groupby(['insert_loop_iterations', 'time_step'])['cuckoo_tables_size'].agg(['mean', 'std']).reset_index()

# Create a pivot table for mean and std
mean_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='mean')
std_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='std')

# Plot the data
plt.figure(figsize=(12, 8))

for col in mean_pivot.columns:
    # Plot mean with a solid line
    plt.plot(mean_pivot.index, mean_pivot[col], label=f'insert_loop_iterations={col}')
    
    # Plot standard deviation as a shaded area
    # plt.fill_between(mean_pivot.index, 
    #                  mean_pivot[col] - std_pivot[col], 
    #                  mean_pivot[col] + std_pivot[col], 
    #                  alpha=0.2)

plt.xlabel('Number of Elements Inserted')
plt.ylabel('Cuckoo Tables Size')
plt.legend()
plt.grid(True)

# Save the plot
output_file = folder + "plots/bin_capacity_{}/cuckoo_tables_size_bin_cap_{}.png".format(bin_capacity, bin_capacity)
plt.savefig(output_file, dpi=300)
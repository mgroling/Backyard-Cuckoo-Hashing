import pandas as pd
import matplotlib.pyplot as plt

bin_capacities = [1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64]
cuckoo_table_sizes = [2453, 1804, 1303, 1071, 930, 762, 661, 542, 702, 384, 333]

def plot_cuckoo_tables_size(bin_capacity):
    folder = "experiments/auxiliary_structures_size/"
    experiment_index = bin_capacities.index(bin_capacity)
    cuckoo_table_size = cuckoo_table_sizes[experiment_index] * 2

    # Read the CSV file
    file_path = folder + "data/data_bin_cap_{}.csv".format(bin_capacity)
    data = pd.read_csv(file_path)

    # Group by 'insert_loop_iterations' and 'time_step', calculate the mean and std of 'queue_size'
    grouped_data = data.groupby(['insert_loop_iterations', 'time_step'])['cuckoo_tables_size'].agg(['mean', 'std']).reset_index()

    # Create a pivot table for mean and std
    mean_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='mean')
    # std_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='std')

    # Plot the data
    plt.figure(figsize=(12, 8))

    for col in mean_pivot.columns:
        # Plot mean with a solid line
        plt.plot(mean_pivot.index, mean_pivot[col] / cuckoo_table_size * 100, label=f'insert_loop_iterations={col}')
        
        # Plot standard deviation as a shaded area
        # plt.fill_between(mean_pivot.index, 
        #                  mean_pivot[col] - std_pivot[col], 
        #                  mean_pivot[col] + std_pivot[col], 
        #                  alpha=0.2)

    plt.yticks(range(0, 71, 5))
    plt.xticks(range(4000, 10001, 500))
    plt.xlabel('Number of Elements Inserted Into Backyard Construction', fontsize = 12)
    plt.ylabel('Average Load Factor Cuckoo Tables Over Repetitions'.format(cuckoo_table_size), fontsize = 12)
    plt.legend()
    plt.grid(True)
    plt.xlim((4000, 10000))

    # Save the plot
    output_file = folder + "plots/bin_capacity_{}/cuckoo_tables_size_bin_cap_{}.png".format(bin_capacity, bin_capacity)
    plt.savefig(output_file, dpi=300)

if __name__ == "__main__":
    for bin_capacity in bin_capacities:
        plot_cuckoo_tables_size(bin_capacity)
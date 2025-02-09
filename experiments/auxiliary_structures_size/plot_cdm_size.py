import pandas as pd
import matplotlib.pyplot as plt

def plot_cdm_size(bin_capacity):
    folder = "experiments/auxiliary_structures_size/"

    # Read the CSV file
    file_path = folder + "data/data_bin_cap_{}.csv".format(bin_capacity)
    data = pd.read_csv(file_path)

    # Group by 'insert_loop_iterations' and 'time_step', calculate the mean and std of 'cdm_size'
    grouped_data = data.groupby(['insert_loop_iterations', 'time_step'])['cdm_size'].agg(['max', 'mean', 'std']).reset_index()

    # Create a pivot table for mean and std
    max_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='max')
    mean_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='mean')
    std_pivot = grouped_data.pivot(index='time_step', columns='insert_loop_iterations', values='std')

    # Plot the data
    plt.figure(figsize=(12, 8))

    for col in max_pivot.columns:
        # Plot mean with a solid line
        plt.plot(mean_pivot.index, max_pivot[col], label=f'insert_loop_iterations={col}')
        
        # Plot standard deviation as a shaded area
        # plt.fill_between(mean_pivot.index, 
        #                 mean_pivot[col] - std_pivot[col], 
        #                 mean_pivot[col] + std_pivot[col], 
        #                 alpha=0.2)

    plt.yticks(range(0, int(max_pivot.max().max()) + 26, 25))
    plt.xticks(range(8000, 10001, 100))
    plt.xlim((8000, 10000))
    plt.xlabel('Number of Elements Inserted Into Backyard Construction', fontsize = 12)
    plt.ylabel('Maximum Cycle Detection Mechanism Size Over Repetitions', fontsize = 12)
    plt.legend()
    plt.grid(True)

    # Save the plot
    output_file = folder + "plots/bin_capacity_{}/cdm_size_bin_cap_{}.png".format(bin_capacity, bin_capacity)
    plt.savefig(output_file, dpi=300)

if __name__ == "__main__":
    for bin_capacity in [1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64]:
        plot_cdm_size(bin_capacity)
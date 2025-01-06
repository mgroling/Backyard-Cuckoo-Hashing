import pandas as pd
import matplotlib.pyplot as plt

folder = "experiments/queue_cdm_cuckoo_tables_size/"
bin_capacity = 12

# Read the CSV file
file_path = folder + "data/data_bin_cap_{}.csv".format(bin_capacity)
data = pd.read_csv(file_path)

print("Maximum cdm size: ", data["cdm_size"].max())
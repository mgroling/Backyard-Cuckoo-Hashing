#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <cmath>
#include <numeric>
#include <utility>
#include <fstream>
#include <unordered_set>
#include "../../src/backyard.h"

std::vector<uint32_t> create_random_input_sequence(int num_elements, int seed)
{
    std::mt19937 gen(seed);
    std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);
    std::unordered_set<uint32_t> elems;

    for (int i = 0; i < num_elements; ++i)
    {
        uint32_t value = dis(gen);
        while (elems.find(value) != elems.end())
        {
            value = dis(gen);
        }
        elems.insert(value);
    }

    return std::vector<uint32_t>(elems.begin(), elems.end());
}

int main()
{
    constexpr int experiment = 10;
    constexpr std::array<int, 11> bin_capacities{1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64};
    // num overflowing elements * 2/3 from plot_tornado_random.png in balls into bins experiment
    constexpr std::array<int, 11> sizes_cuckoo_tables{2453, 1804, 1303, 1071, 930, 762, 661, 542, 702, 384, 333};
    constexpr int num_insertions = 10000;
    constexpr int num_repetitions = 100;
    constexpr int bin_capacity = bin_capacities[experiment];
    constexpr int num_bins = num_insertions / bin_capacity;
    constexpr int size_cuckoo_tables = sizes_cuckoo_tables[experiment];
    // max load factor 3/4 of cuckoo tables (total space) based on balls into bins experiment
    constexpr int n_queue = 200;
    constexpr int k_queue = 20;
    constexpr int num_elems_cdm = 500;
    constexpr int n_cdm = 200;
    constexpr int k_cdm = 20;

    // Open the output CSV file
    std::string file_name = "data/data_bin_cap_";
    file_name += std::to_string(bin_capacity);
    file_name += ".csv";
    std::ofstream csv_file(file_name);
    if (!csv_file.is_open())
    {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }
    // Write the CSV header
    csv_file << "bin_capacity,max_size_cuckoo_tables,insert_loop_iterations,num_repetition,time_step,bins_size,queue_size,cdm_size,cuckoo_tables_size\n";

    std::vector<int> insert_loop_iterations{2, 4, 8, 12, 16, 20};
    std::srand(42);

    for (int i = 0; i < num_repetitions; ++i)
    {
        std::vector<uint32_t> input_sequence = create_random_input_sequence(num_insertions, std::rand());
        for (int iterations : insert_loop_iterations)
        {
            BackyardCuckooHashing<uint32_t, num_bins, bin_capacity, size_cuckoo_tables, n_queue, k_queue, num_elems_cdm, n_cdm, k_cdm>
                backyard(iterations);
            for (int j = 0; j < input_sequence.size(); ++j)
            {
                backyard.insert(input_sequence[j]);
                int backyard_size = backyard.size();
                int bins_queue_size = backyard.bins.size() + backyard.queue.size();
                csv_file << bin_capacity << "," << size_cuckoo_tables << "," << iterations << "," << i << ","
                         << j << "," << backyard.bins.size() << "," << backyard.queue.size() << ","
                         << backyard.cdm.size() << "," << backyard_size - bins_queue_size << "\n";
            }
        }
    }

    // // Write the data to the CSV file
    // Close the file
    csv_file.close();

    return 1;
}
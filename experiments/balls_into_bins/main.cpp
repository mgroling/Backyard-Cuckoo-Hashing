#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <numeric>
#include <utility>
#include <fstream>
#include <unordered_set>
#include "../../src/hash.h"

std::pair<double, double> calculate_average_and_stddev(const std::vector<int> &elements)
{
    if (elements.empty())
    {
        return {0.0, 0.0}; // Return zero for both average and stddev if the vector is empty
    }

    // Calculate the average
    double sum = std::accumulate(elements.begin(), elements.end(), 0.0);
    double average = sum / elements.size();

    // Calculate the standard deviation
    double variance = 0.0;
    for (int element : elements)
    {
        variance += (element - average) * (element - average);
    }
    variance /= elements.size();
    double stddev = std::sqrt(variance);

    return {average, stddev};
}

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

std::vector<uint32_t> create_range_sequence(int num_elements)
{
    std::vector<uint32_t> elems(num_elements);
    std::iota(elems.begin(), elems.end(), 0);
    return elems;
}

std::vector<uint32_t> create_divides_by_sequence(int num_elements, int divisor)
{
    std::vector<uint32_t> elems(num_elements);
    uint32_t num = 0;

    for (int i = 0; i < num_elements; i++)
    {
        elems[i] = num;
        num += divisor;
    }

    return elems;
}

int calculate_number_of_overflowing_elements_balls_into_bins(int num_bins, int bin_capacity, std::vector<uint32_t> input_sequence)
{
    TornadoHash<uint32_t> h = TornadoHash<uint32_t>();
    h.set_range(num_bins);
    std::vector<int> bins(num_bins, bin_capacity);

    uint32_t hash_value;
    int num_overflowing_elements = 0;
    for (uint32_t value : input_sequence)
    {
        hash_value = h.hash(value);
        if (bins[hash_value])
        {
            bins[hash_value]--;
        }
        else
        {
            num_overflowing_elements++;
        }
    }

    return num_overflowing_elements;
}

int main()
{
    // Open the output CSV file
    std::ofstream csv_file("data.csv");
    if (!csv_file.is_open())
    {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }
    // Write the CSV header
    csv_file << "load_factor,bin_capacity,num_bins,average,stddev\n";

    int num_repetitions = 100;
    int num_elements = 100000;

    std::vector<int> bin_capacities{1, 2, 4, 6, 8, 12, 16, 24, 32, 48, 64};
    std::vector<double> load_factors{1.0, 0.95, 0.9, 0.8};

    std::vector<uint32_t> input_sequence = create_divides_by_sequence(num_elements, 7);

    for (double load_factor : load_factors)
    {
        for (int bin_capacity : bin_capacities)
        {
            std::vector<int> results(num_repetitions, 0);
            int num_bins = num_elements / (bin_capacity * load_factor);

            for (int i = 0; i < num_repetitions; i++)
            {
                results[i] = calculate_number_of_overflowing_elements_balls_into_bins(
                    num_bins, bin_capacity, input_sequence);
            }
            std::pair<double, double> result = calculate_average_and_stddev(results);
            double average_number_of_overflowing_elemtents = result.first;
            double stddev_number_of_overflowing_elements = result.second;

            // Write the data to the CSV file
            csv_file << load_factor << "," << bin_capacity << "," << num_bins << ","
                     << average_number_of_overflowing_elemtents << "," << stddev_number_of_overflowing_elements << "\n";
        }
    }
    // Close the file
    csv_file.close();

    return 1;
}
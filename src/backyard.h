#ifndef backyard_
#define backyard_

#include <cstddef>
#include <array>
#include <optional>

#include "hash.h"
#include "cdm.h"
#include "queue.h"
#include "simple_bin.h"

// Custom operator between optional<T> and T that only returns true
// if the optional has a value and that equals the other one
template <typename T>
bool operator==(const std::optional<T> &opt, const T &value)
{
    // If the optional is empty, return false
    if (!opt.has_value())
    {
        return false;
    }
    // Otherwise, compare the value inside the optional
    return opt.value() == value;
}

template <typename T, int num_bins, int bin_capacity, int size_cuckoo_tables, int n_queue, int k_queue,
          int num_elems_cdm, int n_cdm, int k_cdm>
class BackyardCuckooHashing
{
public:
    BackyardCuckooHashing(int insert_loop_iterations)
    {
        this->insert_loop_iterations = insert_loop_iterations;
        bins.fill(SimpleBin<T, bin_capacity>{});
        bins_h.set_range(num_bins);
        cuckoo_tables_h[0].set_range(size_cuckoo_tables);
        cuckoo_tables_h[1].set_range(size_cuckoo_tables);
    }

    bool contains(const T &item) const
    {
        T temp1;
        T temp2;
        if (cuckoo_tables[0][cuckoo_tables_h[0].hash(item)].has_value())
        {
            temp1 = cuckoo_tables[0][cuckoo_tables_h[0].hash(item)].value();
        }
        if (cuckoo_tables[1][cuckoo_tables_h[1].hash(item)].has_value())
        {
            temp2 = cuckoo_tables[1][cuckoo_tables_h[1].hash(item)].value();
        }
        bool isContained = bins[bins_h.hash(item)].contains(item) ||
                           cuckoo_tables[0][cuckoo_tables_h[0].hash(item)] == item ||
                           cuckoo_tables[1][cuckoo_tables_h[1].hash(item)] == item ||
                           queue.contains({item, true}) || queue.contains({item, false});
        return bins[bins_h.hash(item)].contains(item) ||
               cuckoo_tables[0][cuckoo_tables_h[0].hash(item)] == item ||
               cuckoo_tables[1][cuckoo_tables_h[1].hash(item)] == item ||
               queue.contains({item, true}) || queue.contains({item, false});
    }

    bool remove(const T &item)
    {
        if (bins[bins_h.hash(item)].remove(item))
        {
            return true;
        }
        uint64_t hash = cuckoo_tables_h[0].hash(item);
        if (cuckoo_tables[0][hash] == item)
        {
            cuckoo_tables[0][hash].reset();
            return true;
        }
        hash = cuckoo_tables_h[1].hash(item);
        if (cuckoo_tables[1][hash] == item)
        {
            cuckoo_tables[1][hash].reset();
            return true;
        }
        if (queue.remove({item, true}))
        {
            return true;
        }
        if (queue.remove({item, false}))
        {
            return true;
        }
        return false;
    }

    void insert(const T &item)
    {
        queue.push_back({item, true});
        std::optional<T> y;
        bool b = true;
        uint64_t hash = 0;
        for (int i = 0; i < insert_loop_iterations; ++i)
        {
            if (!y.has_value())
            {
                if (queue.empty())
                {
                    return;
                }
                else
                {
                    const std::pair<T, bool> temp = queue.pop_front().value();
                    y = temp.first;
                    b = temp.second;
                }
            }
            hash = bins_h.hash(y.value());
            if (bins[hash].has_space()) // bin has space, move element to first level
            {
                bins[hash].insert(y.value());
                y.reset();
            }
            else
            {
                hash = cuckoo_tables_h[b].hash(y.value());
                if (!cuckoo_tables[b][hash].has_value())
                {
                    cuckoo_tables[b][hash] = y;
                    cdm.reset();
                    y.reset();
                }
                else
                {
                    if (cdm.contains({y.value(), b}))
                    {
                        queue.push_back({y.value(), b});
                        cdm.reset();
                        y.reset();
                    }
                    else
                    {
                        T z = cuckoo_tables[b][hash].value();
                        cuckoo_tables[b][hash] = y;
                        cdm.insert({y.value(), b});
                        y = z;
                        b = !b;
                    }
                }
            }
        }

        if (y.has_value())
        {
            queue.push_front({y.value(), b});
        }
    }

private:
    ConstantTimeQueue<std::pair<T, bool>, n_queue, k_queue> queue;
    ConstantTimeCollection<std::pair<T, bool>, num_elems_cdm, n_cdm, k_cdm> cdm;
    PairwiseIndependentHashFunction<uint64_t> bins_h;
    std::array<SimpleBin<T, bin_capacity>, num_bins> bins;
    std::array<PairwiseIndependentHashFunction<uint64_t>, 2> cuckoo_tables_h;
    std::array<std::array<std::optional<T>, size_cuckoo_tables>, 2> cuckoo_tables;
    int insert_loop_iterations;
};

#endif
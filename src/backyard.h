#ifndef cdm_
#define cdm_

#include <cstddef>
#include <array>

#include "hash.h"
#include "cdm.h"
#include "queue.h"
#include "simple_bin.h"

template <typename T, int num_elements, int num_bins, int bin_capacity, int n_queue, int k_queue, int num_elems_cdm, int n_cdm, int k_cdm>
class BackyardCuckooHashing
{
public:
    BackyardCuckooHashing()
    {
        bins.fill(SimpleBin{});
    }

private:
    ConstantTimeQueue<T, n_queue, k_queue> queue;
    CycleDetectionMechanism<T, num_elems_cdm, n_cdm, k_cdm> cdm;
    std::array<SimpleBin, num_bins> bins;
};

#endif
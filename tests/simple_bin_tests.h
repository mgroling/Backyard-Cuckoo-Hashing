#include <cassert>
#include <cstdint>
#include <unordered_set>
#include "../src/simple_bin.h"

void test_simple_bin_initial_state()
{
    SimpleBin<uint64_t, 5> bin;
    assert(bin.size() == 0);
}

void test_simple_bin_insertion()
{
    SimpleBin<uint64_t, 5> bin;
    assert(bin.insert(10));
    assert(bin.size() == 1);
    assert(bin.contains(10));

    assert(bin.insert(20));
    assert(bin.insert(30));
    assert(bin.size() == 3);
    assert(bin.contains(20) && bin.contains(30));
}

void test_simple_bin_removal()
{
    SimpleBin<uint64_t, 5> bin;
    bin.insert(10);
    bin.insert(20);
    bin.insert(30);

    assert(bin.remove(20));
    assert(bin.size() == 2);
    assert(!bin.contains(20));

    assert(!bin.remove(40));
    assert(bin.size() == 2);
}

void test_simple_bin_capacity_limit()
{
    SimpleBin<uint64_t, 3> bin;
    assert(bin.insert(10));
    assert(bin.insert(20));
    assert(bin.insert(30));
    assert(!bin.insert(40));
    assert(bin.size() == 3);
}

void test_simple_bin_reuse_deleted_slots()
{
    SimpleBin<uint64_t, 5> bin;
    bin.insert(10);
    bin.insert(20);

    assert(bin.remove(10));
    assert(bin.size() == 1);

    assert(bin.insert(30));
    assert(bin.size() == 2);
    assert(bin.contains(30));
}

void test_simple_bin_remove_all()
{
    SimpleBin<uint64_t, 5> bin;
    bin.insert(10);
    bin.insert(20);
    bin.insert(30);

    assert(bin.remove(10));
    assert(bin.remove(20));
    assert(bin.remove(30));
    assert(bin.size() == 0);
}

void test_bin_collection_insertion()
{
    SimpleBinCollection<uint32_t, 5, 5> bins;
    assert(bins.insert(10));
    assert(bins.size() == 1);
    assert(bins.contains(10));

    assert(bins.insert(20));
    assert(bins.insert(30));
    assert(bins.size() == 3);
    assert(bins.contains(20) && bins.contains(30));
}

void test_bin_collection_random_operations()
{
    SimpleBinCollection<uint32_t, 100, 100> collection;
    std::unordered_multiset<uint32_t> std_collection;

    std::srand(42);

    // Perform a series of random operations
    for (int i = 0; i < 10000; ++i)
    {
        int operation = std::rand() % 3;
        uint32_t value = std::rand() % 1000;

        if (operation == 0)
        {
            if (collection.insert(value))
            {
                // only insert into standard collection if there was space
                std_collection.insert(value);
            }
        }
        else if (operation == 1)
        {
            bool std_contains_value = std_collection.count(value) > 0;
            int std_num_instances_removed = std_collection.erase(value);
            int num_instances_removed = 0;
            // remove all instance of value, since this is what multiset does
            while (collection.remove(value))
            {
                ++num_instances_removed;
            }
            assert(num_instances_removed == std_num_instances_removed);
        }
        else
        {
            assert(collection.contains(value) == (std_collection.count(value) > 0));
        }
        assert(collection.size() == std_collection.size());
    }
}
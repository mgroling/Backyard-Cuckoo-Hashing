#include <cassert>
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include "../src/backyard.h"

void test_backyard_insert_and_contains()
{
    // Create an instance of the BackyardCuckooHashing class
    BackyardCuckooHashing<uint32_t, 5, 2, 4, 5, 3, 10, 5, 3> dictionary(5);

    // Insert elements and check if they are contained
    dictionary.insert(42);
    dictionary.insert(13);
    dictionary.insert(7);

    assert(dictionary.contains(42));
    assert(dictionary.contains(13));
    assert(dictionary.contains(7));
    assert(!dictionary.contains(99)); // Element not inserted
}

void test_backyard_remove()
{
    BackyardCuckooHashing<uint32_t, 5, 2, 4, 5, 3, 10, 5, 3> dictionary(5);

    // Insert elements
    dictionary.insert(42);
    dictionary.insert(13);

    // Remove an element
    assert(dictionary.remove(42));
    assert(!dictionary.contains(42)); // Ensure it's no longer in the table
    assert(dictionary.contains(13));  // Other elements should still be present

    // Remove an element that doesn't exist
    assert(!dictionary.remove(99)); // Should return false
}

void test_backyard_overflow_handling()
{
    BackyardCuckooHashing<uint32_t, 5, 2, 4, 5, 3, 10, 5, 3> dictionary(5);

    // Insert more elements than the bins can handle
    for (uint32_t i = 0; i < 15; ++i)
    {
        dictionary.insert(i);
    }

    // Check that all inserted elements are contained
    for (uint32_t i = 0; i < 15; ++i)
    {
        assert(dictionary.contains(i));
    }

    // Check an element that wasn't inserted
    assert(!dictionary.contains(999));
}

void test_backyard_alternating_operations()
{
    BackyardCuckooHashing<uint32_t, 5, 2, 4, 5, 3, 10, 5, 3> dictionary(5);

    // Perform alternating insertions and removals
    dictionary.insert(10);
    dictionary.insert(20);
    assert(dictionary.contains(10));
    assert(dictionary.contains(20));

    dictionary.remove(10);
    assert(!dictionary.contains(10));
    assert(dictionary.contains(20));

    dictionary.insert(30);
    assert(dictionary.contains(30));
    assert(dictionary.contains(20));

    dictionary.remove(20);
    dictionary.remove(30);
    assert(!dictionary.contains(20));
    assert(!dictionary.contains(30));
}

void test_backyard_reset_behavior()
{
    BackyardCuckooHashing<uint32_t, 5, 2, 4, 5, 3, 10, 5, 3> dictionary(5);

    // Insert some elements
    dictionary.insert(1);
    dictionary.insert(2);
    dictionary.insert(3);

    // Check their presence
    assert(dictionary.contains(1));
    assert(dictionary.contains(2));
    assert(dictionary.contains(3));

    // Remove elements and verify they are gone
    dictionary.remove(1);
    dictionary.remove(2);
    dictionary.remove(3);
    assert(!dictionary.contains(1));
    assert(!dictionary.contains(2));
    assert(!dictionary.contains(3));

    // Insert new elements after removal
    dictionary.insert(4);
    dictionary.insert(5);
    assert(dictionary.contains(4));
    assert(dictionary.contains(5));
}

void test_backyard_random_operations()
{
    constexpr int num_operations = 10000;

    // Create instances of your custom set and the standard unordered_set
    constexpr int num_bins = 100;
    constexpr int bin_capacity = 10;
    constexpr int size_cuckoo_tables = 100;
    constexpr int n_queue = 50;
    constexpr int k_queue = 10;
    constexpr int num_elems_cdm = 100;
    constexpr int n_cdm = 50;
    constexpr int k_cdm = 10;
    constexpr int num_iterations_per_insertion = 10;
    BackyardCuckooHashing<uint32_t, num_bins, bin_capacity, size_cuckoo_tables, n_queue, k_queue, num_elems_cdm, n_cdm, k_cdm> custom_set(10);
    std::unordered_set<uint32_t> std_set;

    // Seed the random number generator
    std::srand(42);

    // Number of operations to perform

    for (int i = 0; i < num_operations; ++i)
    {
        int operation = std::rand() % 3; // 0: insert, 1: remove, 2: contains
        int value = std::rand() % 1000;  // Random value between 0 and 999

        if (operation == 0)
        {
            custom_set.insert(value);
            std_set.insert(value);
        }
        else if (operation == 1)
        {
            bool custom_remove = custom_set.remove(value);
            bool std_remove = std_set.erase(value) > 0;
            assert(custom_remove == std_remove);
        }
        else if (operation == 2)
        {
            bool custom_contains = custom_set.contains(value);
            bool std_contains = (std_set.count(value) > 0);
            assert(custom_contains == std_contains);
        }
    }
}
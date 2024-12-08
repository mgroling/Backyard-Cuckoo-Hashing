#include <cassert>
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include "../src/hash.h"

void test_hash_set_range()
{
    PairwiseIndependentHashFunction<uint64_t> hash_func = PairwiseIndependentHashFunction<uint64_t>();
    hash_func.set_range(100); // Set the range to 100

    uint64_t value = 42;
    uint64_t hash_value = hash_func.hash(value);

    assert(hash_value < 100); // Ensure hash value is within the specified range
}

void test_hash_randomize_parameters()
{
    PairwiseIndependentHashFunction<uint64_t> hash_func = PairwiseIndependentHashFunction<uint64_t>();
    hash_func.set_range(1000000000000UL); // Set the range for consistent comparison

    uint64_t value = 12345;
    uint64_t first_hash = hash_func.hash(value);

    hash_func.randomize_parameters(); // Randomize the parameters

    uint64_t second_hash = hash_func.hash(value);

    // Hash values should be different after parameter randomization
    assert(first_hash != second_hash);
}

void test_hash_hash_distribution()
{
    PairwiseIndependentHashFunction<uint64_t> hash_func = PairwiseIndependentHashFunction<uint64_t>();
    hash_func.set_range(100); // Setting range to 100 to observe distribution

    std::unordered_set<uint64_t> unique_hashes;
    for (uint64_t i = 0; i < 1000; ++i)
    {
        unique_hashes.insert(hash_func.hash(i));
    }

    // We expect at least some diversity in the hash values within the range
    assert(unique_hashes.size() > 50); // Arbitrary threshold for uniqueness
}

void test_hash_collision_rate()
{
    PairwiseIndependentHashFunction<uint64_t> hash_func = PairwiseIndependentHashFunction<uint64_t>();
    hash_func.set_range(10); // Small range to induce collisions

    std::unordered_set<uint64_t> hash_values;
    int collisions = 0;
    for (uint64_t i = 0; i < 100; ++i)
    {
        uint64_t hash_value = hash_func.hash(i);
        if (hash_values.find(hash_value) != hash_values.end())
        {
            collisions++;
        }
        hash_values.insert(hash_value);
    }

    // Since the range is small, some collisions are expected
    assert(collisions > 0);
}
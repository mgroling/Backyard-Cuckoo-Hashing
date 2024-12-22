#include <cassert>
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include "../src/hash.h"

// Test 1: Ensure the hash value is within the specified range
void test_hash_set_range()
{
    TornadoHash<uint32_t> hash_func;
    hash_func.set_range(100); // Set the range to 100

    uint32_t value = 42;
    uint64_t hash_value = hash_func.hash(value);

    assert(hash_value < 100); // Ensure hash value is within the specified range
}

// Test 2: Ensure different parameters produce different hash outputs
void test_randomize_parameters()
{
    TornadoHash<uint32_t> hash_func1;
    hash_func1.set_range(UINT64_MAX);
    TornadoHash<uint32_t> hash_func2;
    hash_func2.set_range(UINT64_MAX);

    uint32_t value = 12345;

    uint64_t hash_value1 = hash_func1.hash(value);
    uint64_t hash_value2 = hash_func2.hash(value);

    // Probability of collision is negligible if random_bits are truly randomized
    assert(hash_value1 != hash_value2);
}

// Test 3: Ensure the same object produces consistent hash outputs
void test_consistent_hashing()
{
    TornadoHash<uint32_t> hash_func;
    hash_func.set_range(1000);

    uint32_t value = 54321;

    uint64_t hash_value1 = hash_func.hash(value);
    uint64_t hash_value2 = hash_func.hash(value);

    assert(hash_value1 == hash_value2); // Hashing the same input with the same object should produce the same output
}

// Test 4: Verify uniqueness of hashes for small input space
void test_hash_uniqueness()
{
    TornadoHash<uint32_t> hash_func;
    hash_func.set_range(UINT64_MAX); // Large range to avoid modulus collisions

    std::unordered_set<uint64_t> hash_values;

    for (uint32_t i = 0; i < 1000; ++i)
    {
        uint64_t hash_value = hash_func.hash(i);
        assert(hash_values.find(hash_value) == hash_values.end()); // Ensure no duplicate hashes
        hash_values.insert(hash_value);
    }
}

// Test 5: Verify the hash function works for edge case inputs
void test_hash_edge_cases()
{
    TornadoHash<uint32_t> hash_func;
    hash_func.set_range(UINT64_MAX);

    uint32_t values[] = {0, 1, UINT32_MAX};

    for (uint32_t value : values)
    {
        uint64_t hash_value = hash_func.hash(value);
        // Just ensure no crashes; optionally, print or log `hash_value` for inspection
        (void)hash_value; // Avoid unused variable warnings
    }
}
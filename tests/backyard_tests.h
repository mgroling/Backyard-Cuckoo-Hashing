#include <cassert>
#include <cstddef>
#include <cstdint>
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
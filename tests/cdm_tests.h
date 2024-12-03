#include <cassert>
#include "../src/cdm.h"

void test_insert_and_contains()
{
    CycleDetectionMechanism<uint64_t, 3, 5, 3> cdm;

    // Insert elements and check if they are contained in the cdm
    cdm.insert(42);
    cdm.insert(13);
    cdm.insert(7);

    assert(cdm.contains(42));
    assert(cdm.contains(13));
    assert(cdm.contains(7));
    assert(!cdm.contains(99)); // Element not inserted
}

void test_rebuild_trigger()
{
    CycleDetectionMechanism<uint64_t, 9, 3, 3> cdm;

    // Fill the cdm to the max, which will very likely force multiple rebuilds
    for (uint64_t i = 0; i < 9; ++i)
    {
        cdm.insert(i);
    }

    // Check that all inserted elements are present
    for (uint64_t i = 0; i < 9; ++i)
    {
        assert(cdm.contains(i));
    }

    // Ensure an element not in the cdm is still not found
    assert(!cdm.contains(20));
}

void test_reset()
{
    CycleDetectionMechanism<uint64_t, 10, 5, 3> cdm;

    // Insert some elements
    cdm.insert(42);
    cdm.insert(13);

    // Reset the data structure
    cdm.reset();

    // Check that the cdm is empty
    assert(cdm.empty());
    assert(!cdm.contains(42));
    assert(!cdm.contains(13));

    // Insert some elements
    cdm.insert(51);
    cdm.insert(13);

    assert(!cdm.empty());
    assert(cdm.contains(51));
    assert(cdm.contains(13));
}

void test_empty()
{
    CycleDetectionMechanism<uint64_t, 10, 5, 3> cdm;

    // Initially, the cdm should be empty
    assert(cdm.empty());

    // Insert an element and check that it's no longer empty
    cdm.insert(1);
    assert(!cdm.empty());
}

void test_duplicate_inserts()
{
    CycleDetectionMechanism<uint64_t, 10, 5, 3> cdm;

    // Insert the same element multiple times
    cdm.insert(42);
    cdm.insert(42);
    cdm.insert(42);

    // Ensure the element is only present once and no issues arise
    assert(cdm.contains(42));

    // Check the structure remains functional
    cdm.insert(13);
    assert(cdm.contains(13));
    cdm.reset();
    assert(!cdm.contains(42));
    assert(!cdm.contains(13));
}

void test_alternating_inserts_and_resets()
{
    CycleDetectionMechanism<uint64_t, 10, 5, 3> cdm;

    for (int round = 0; round < 5; ++round)
    {
        // Insert elements in each round
        for (uint64_t i = 0; i < 5; ++i)
        {
            cdm.insert(i + round * 10);
        }

        // Verify elements
        for (uint64_t i = 0; i < 5; ++i)
        {
            assert(cdm.contains(i + round * 10));
        }

        // Reset and ensure it's empty
        cdm.reset();
        for (uint64_t i = 0; i < 5; ++i)
        {
            assert(!cdm.contains(i + round * 10));
        }
    }
}
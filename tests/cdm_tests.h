#include <cassert>
#include <cstddef>
#include <cstdint>
#include "../src/cdm.h"

void test_cdm_insert_and_contains_three_times_checked()
{
    CycleDetectionMechanism<std::pair<uint64_t, bool>, 5, 5, 3> cdm;

    assert(!cdm.contains({1, true}));
    cdm.insert({1, true});
    assert(!cdm.contains({2, false}));
    cdm.insert({2, false});
    assert(!cdm.contains({1, true})); // first time this element is checked (when present), so should still be false
    cdm.insert({1, true});
    assert(!cdm.contains({3, true}));
    cdm.insert({1, true});
    assert(cdm.contains({1, true}));
}

void test_cdm_insert_and_contains_two_times_checked()
{
    CycleDetectionMechanism<std::pair<uint64_t, bool>, 5, 5, 3> cdm;

    assert(!cdm.contains({1, true}));
    cdm.insert({1, true});
    assert(!cdm.contains({2, false}));
    cdm.insert({2, false});
    assert(!cdm.contains({1, true})); // first time this element is checked (when present), so should still be false
    cdm.insert({1, true});
    assert(!cdm.contains({3, true}));
    cdm.insert({1, true});
    assert(cdm.contains({2, false}));
}

void test_cdm_reset()
{
    CycleDetectionMechanism<std::pair<uint64_t, bool>, 5, 5, 3> cdm;

    assert(!cdm.contains({1, true}));
    cdm.insert({1, true});
    assert(!cdm.contains({2, false}));
    cdm.insert({2, false});
    assert(!cdm.contains({1, true})); // first time this element is checked (when present), so should still be false
    cdm.insert({1, true});
    assert(!cdm.contains({3, true}));
    cdm.insert({1, true});
    assert(cdm.contains({1, true}));

    cdm.reset();

    assert(!cdm.contains({1, true}));
    cdm.insert({1, true});
    assert(!cdm.contains({2, false}));
    cdm.insert({2, false});
    assert(!cdm.contains({1, true})); // first time this element is checked (when present), so should still be false
    cdm.insert({1, true});
    assert(!cdm.contains({3, true}));
    cdm.insert({1, true});
    assert(cdm.contains({2, false}));
}

void test_collection_insert_and_contains()
{
    ConstantTimeCollection<uint64_t, 3, 5, 3> collection;

    // Insert elements and check if they are contained in the collection
    collection.insert(42);
    collection.insert(13);
    collection.insert(7);

    assert(collection.contains(42));
    assert(collection.contains(13));
    assert(collection.contains(7));
    assert(!collection.contains(99)); // Element not inserted
}

void test_collection_rebuild_trigger()
{
    ConstantTimeCollection<uint64_t, 9, 3, 3> collection;

    // Fill the collection to the max, which will very likely force multiple rebuilds
    for (uint64_t i = 0; i < 9; ++i)
    {
        collection.insert(i);
    }

    // Check that all inserted elements are present
    for (uint64_t i = 0; i < 9; ++i)
    {
        assert(collection.contains(i));
    }

    // Ensure an element not in the collection is still not found
    assert(!collection.contains(20));
}

void test_collection_recollection()
{
    ConstantTimeCollection<uint64_t, 10, 5, 3> collection;

    // Insert some elements
    collection.insert(42);
    collection.insert(13);

    // Recollection the data structure
    collection.reset();

    // Check that the collection is empty
    assert(collection.empty());
    assert(!collection.contains(42));
    assert(!collection.contains(13));

    // Insert some elements
    collection.insert(51);
    collection.insert(13);

    assert(!collection.empty());
    assert(collection.contains(51));
    assert(collection.contains(13));
}

void test_collection_empty()
{
    ConstantTimeCollection<uint64_t, 10, 5, 3> collection;

    // Initially, the collection should be empty
    assert(collection.empty());

    // Insert an element and check that it's no longer empty
    collection.insert(1);
    assert(!collection.empty());
}

void test_cdm_duplicate_inserts()
{
    ConstantTimeCollection<uint64_t, 10, 5, 3> collection;

    // Insert the same element multiple times
    collection.insert(42);
    collection.insert(42);
    collection.insert(42);

    // Ensure the element is only present once and no issues arise
    assert(collection.contains(42));

    // Check the structure remains functional
    collection.insert(13);
    assert(collection.contains(13));
    collection.reset();
    assert(!collection.contains(42));
    assert(!collection.contains(13));
}

void test_collection_alternating_inserts_and_recollections()
{
    ConstantTimeCollection<uint64_t, 10, 5, 3> collection;

    for (int round = 0; round < 5; ++round)
    {
        // Insert elements in each round
        for (uint64_t i = 0; i < 5; ++i)
        {
            collection.insert(i + round * 10);
        }

        // Verify elements
        for (uint64_t i = 0; i < 5; ++i)
        {
            assert(collection.contains(i + round * 10));
        }

        // Recollection and ensure it's empty
        collection.reset();
        for (uint64_t i = 0; i < 5; ++i)
        {
            assert(!collection.contains(i + round * 10));
        }
    }
}
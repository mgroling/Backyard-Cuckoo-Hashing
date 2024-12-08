#include <cassert>
#include <cstdint>
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
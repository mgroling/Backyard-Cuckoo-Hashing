#include <cassert>
#include <cstdint>
#include "../src/queue.h"

void test_queue_push_back()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_back(1);
    queue.push_back(2);
    queue.push_back(3);

    assert(queue.contains(1));
    assert(queue.contains(2));
    assert(queue.contains(3));
}

void test_queue_push_front()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_front(1);
    queue.push_front(2);
    queue.push_front(3);

    assert(queue.contains(1));
    assert(queue.contains(2));
    assert(queue.contains(3));
}

void test_queue_pop_front()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_back(1);
    queue.push_back(2);

    auto front = queue.pop_front();
    assert(front.has_value() && front.value() == 1); // Check if the first item is 1
    assert(queue.contains(2));
    assert(!queue.contains(1)); // Item 1 should be "deleted"

    front = queue.pop_front();
    assert(front.has_value() && front.value() == 2); // Check if the next item is 2
    assert(queue.empty());                           // Queue should be empty after popping all items
}

void test_queue_contains()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_back(5);
    queue.push_front(3);

    assert(queue.contains(5));
    assert(queue.contains(3));
    assert(!queue.contains(1)); // Item 1 was never added
}

void test_queue_remove()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_back(10);
    queue.push_back(20);

    assert(queue.contains(10));
    assert(queue.remove(10));
    assert(!queue.contains(10)); // 10 should be removed
    assert(queue.contains(20));  // 20 should still be in the queue
}

void test_queue_empty()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    assert(queue.empty()); // Queue should be empty initially

    queue.push_back(1);
    assert(!queue.empty()); // Queue should not be empty after adding an element

    queue.pop_front();
    assert(queue.empty()); // Queue should be empty after removing the only element
}

void test_queue_rebuild()
{
    ConstantTimeQueue<uint64_t, 2, 3> queue = ConstantTimeQueue<uint64_t, 2, 3>();
    // Fill the queue to the max, which will very likely force multiple rebuilds
    queue.push_front(1);
    queue.push_front(2);
    queue.push_front(3);
    queue.push_front(4);
    queue.push_front(5);
    queue.push_front(6);

    assert(queue.contains(1));
    assert(queue.contains(2));
    assert(queue.contains(3));
    assert(queue.contains(4));
    assert(queue.contains(5));
    assert(queue.contains(6));

    auto front = queue.pop_front();
    assert(front.has_value() && front.value() == 6);
    front = queue.pop_front();
    assert(front.has_value() && front.value() == 5);
    front = queue.pop_front();
    assert(front.has_value() && front.value() == 4);
    front = queue.pop_front();
    assert(front.has_value() && front.value() == 3);
    front = queue.pop_front();
    assert(front.has_value() && front.value() == 2);
    front = queue.pop_front();
    assert(front.has_value() && front.value() == 1);

    assert(queue.empty());
}
#include <cassert>
#include <cstdint>
#include <deque>
#include <unordered_set>
#include <algorithm>
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

    assert(queue.size() == 2);
    assert(queue.contains(5));
    assert(queue.contains(3));
    assert(!queue.contains(1)); // Item 1 was never added
}

void test_queue_remove()
{
    ConstantTimeQueue<uint64_t, 10, 3> queue = ConstantTimeQueue<uint64_t, 10, 3>();
    queue.push_back(10);
    queue.push_back(20);

    assert(queue.size() == 2);
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
    assert(queue.size() == 6);

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

void test_queue_random_operations()
{
    constexpr int n = 1000;
    constexpr int k = 20;
    ConstantTimeQueue<uint32_t, n, k> custom_queue;
    std::deque<uint32_t> ref_deque;

    std::srand(42); // Seed the random number generator
    constexpr int num_operations = 10000;

    for (int i = 0; i < num_operations; ++i)
    {
        if (i == 24)
        {
            int x = 3;
            int y = x + 3;
        }
        int operation = std::rand() % 6; // Random operation
        int value = std::rand() % 100;   // Random value between 0 and 99
        std::vector<uint32_t> custom_queue_as_vector = custom_queue.to_vector();
        std::vector<uint32_t> ref_deque_as_vector(ref_deque.begin(), ref_deque.end());

        if (operation == 0)
        {
            custom_queue.push_back(value);
            ref_deque.push_back(value);
        }
        else if (operation == 1)
        {
            custom_queue.push_front(value);
            ref_deque.push_front(value);
        }
        else if (operation == 2)
        {
            if (!ref_deque.empty())
            {
                auto custom_value = custom_queue.pop_front();
                uint32_t ref_value = ref_deque.front();
                ref_deque.pop_front();

                assert(custom_value.has_value() && custom_value.value() == ref_value);
            }
            else
            {
                assert(!custom_queue.pop_front().has_value());
            }
        }
        else if (operation == 3)
        {
            bool custom_contains = custom_queue.contains(value);
            bool ref_contains = std::find(ref_deque.begin(), ref_deque.end(), value) != ref_deque.end();

            assert(custom_contains == ref_contains);
        }
        else if (operation == 4)
        {
            bool custom_removed = custom_queue.remove(value);
            std::vector<uint32_t> after_removal_custom = custom_queue.to_vector();
            std::vector<uint32_t> before_removal_ref(ref_deque.begin(), ref_deque.end());
            bool ref_removed = std::find(ref_deque.begin(), ref_deque.end(), value) != ref_deque.end();
            if (ref_removed)
            {
                for (int j = 0; j < before_removal_ref.size(); ++j)
                {
                    if (j == after_removal_custom.size() || before_removal_ref[j] != after_removal_custom[j])
                    {
                        assert(before_removal_ref[j] == value);
                        ref_deque.erase(ref_deque.begin() + j);
                        break;
                    }
                }
            }

            assert(custom_removed == ref_removed);
        }
        else if (operation == 5)
        {
            bool custom_empty = custom_queue.empty();
            bool ref_empty = ref_deque.empty();

            assert(custom_empty == ref_empty);
        }

        std::vector<uint32_t> custom_queue_as_vector_2 = custom_queue.to_vector();
        std::vector<uint32_t> ref_deque_as_vector_2(ref_deque.begin(), ref_deque.end());
        assert(custom_queue_as_vector_2 == ref_deque_as_vector_2);
        assert(custom_queue.size() == ref_deque.size());
    }
}
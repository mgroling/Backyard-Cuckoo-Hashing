#ifndef queue_
#define queue_

#include <cstddef>
#include <optional>
#include <array>
#include <vector>

#include "hash.h"

template <typename T>
class Node
{
public:
    T data;
    Node<T> *prev = nullptr;
    Node<T> *next = nullptr;
    bool deleted;

    Node()
    {
        deleted = true;
    }

    Node(T data) : data(data)
    {
        deleted = false;
    }
};

template <typename T, int n, int k>
class ConstantTimeQueue
{
public:
    ConstantTimeQueue()
    {
        for (int i = 0; i < k; ++i)
        {
            h[i].set_range(n);
        }
    }

    void push_back(T item)
    {
        // find an empty spot for the element from the following options:
        // {A_1[h_1(item)], A_2[h_2(item)], .., A_k[h_k(item)]}
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (arrays[position].deleted)
            {
                arrays[position] = Node<T>(item);

                if (tail)
                {
                    tail->next = &arrays[position];
                    arrays[position].prev = tail;
                    tail = &arrays[position];
                    return;
                }

                head = &arrays[position];
                tail = &arrays[position];
                return;
            }
        }

        // didn't find an empty spot for the element => rebuild the whole data structure
        rebuild(item, false);
    }

    void push_front(T item)
    {
        // find an empty spot for the element from the following options:
        // {A_1[h_1(item)], A_2[h_2(item)], .., A_k[h_k(item)]}
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (arrays[position].deleted)
            {
                arrays[position] = Node<T>(item);

                if (head)
                {
                    head->prev = &arrays[position];
                    arrays[position].next = head;
                    head = &arrays[position];
                    return;
                }

                head = &arrays[position];
                tail = &arrays[position];
                return;
            }
        }

        // didn't find an empty spot for the element => rebuild the whole data structure
        rebuild(item, true);
    }

    std::optional<T> pop_front()
    {
        if (empty())
        {
            return std::nullopt;
        }

        T item = head->data;
        head->deleted = true;

        if (head->next)
        {
            head = head->next;
            head->prev = nullptr;
        }
        else
        {
            head = nullptr;
            tail = nullptr;
        }

        return item;
    }

    bool contains(T item)
    {
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (!arrays[position].deleted && arrays[position].data == item)
            {
                return true;
            }
        }

        return false;
    }

    bool remove(T item)
    {
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (!arrays[position].deleted && arrays[position].data == item)
            {
                Node<T> *node = &arrays[position];
                node->deleted = true;

                if (node->prev)
                {
                    node->prev->next = node->next;
                }
                if (node->next)
                {
                    node->next->prev = node->prev;
                }
                return true;
            }
        }

        return false;
    }

    bool empty()
    {
        return head == nullptr;
    }

private:
    std::array<Node<T>, k * n> arrays;
    std::array<PairwiseIndependentHashFunction<T>, k> h;
    Node<T> *head = nullptr;
    Node<T> *tail = nullptr;

    int get_position(int num_array, int array_index)
    {
        return num_array * n + array_index;
    }

    void rebuild(T item, bool place_at_front)
    {
        std::vector<T> items;
        if (place_at_front)
        {
            items.push_back(item);
        }
        // collect all present elements in the datastructure and set them as deleted
        if (head)
        {
            Node<T> *node = head;
            node->deleted = true;
            items.push_back(node->data);
            while (node->next)
            {
                node = node->next;
                node->deleted = true;
                items.push_back(node->data);
            }
        }
        if (!place_at_front)
        {
            items.push_back(item);
        }
        // sample new hash functions
        for (int i = 0; i < k; i++)
        {
            h[i].randomize_parameters();
        }
        head = nullptr;
        tail = nullptr;

        // insert all elements into the data structure again (with new seed), this may call rebuild again
        for (const T &elem : items)
        {
            push_back(elem);
        }
    }
};

#endif
#ifndef queue_
#define queue_

#include <cstddef>
#include <optional>
#include <array>
#include <vector>

#include "hash.h"

template <typename T>
class QueueNode
{
public:
    T data;
    QueueNode<T> *prev = nullptr;
    QueueNode<T> *next = nullptr;
    bool deleted;

    QueueNode()
    {
        deleted = true;
    }

    QueueNode(T data) : data(data)
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

    void push_back(const T &item)
    {
        // find an empty spot for the element from the following options:
        // {A_1[h_1(item)], A_2[h_2(item)], .., A_k[h_k(item)]}
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (arrays[position].deleted)
            {
                arrays[position] = QueueNode<T>(item);

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

    void push_front(const T &item)
    {
        // find an empty spot for the element from the following options:
        // {A_1[h_1(item)], A_2[h_2(item)], .., A_k[h_k(item)]}
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (arrays[position].deleted)
            {
                arrays[position] = QueueNode<T>(item);

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

    bool contains(const T &item) const
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

    bool remove(const T &item)
    {
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            if (!arrays[position].deleted && arrays[position].data == item)
            {
                QueueNode<T> *QueueNode = &arrays[position];
                QueueNode->deleted = true;

                if (QueueNode->prev)
                {
                    QueueNode->prev->next = QueueNode->next;
                }
                if (QueueNode->next)
                {
                    QueueNode->next->prev = QueueNode->prev;
                }
                return true;
            }
        }

        return false;
    }

    bool empty() const
    {
        return head == nullptr;
    }

private:
    std::array<QueueNode<T>, k * n> arrays;
    std::array<PairwiseIndependentHashFunction<T>, k> h;
    QueueNode<T> *head = nullptr;
    QueueNode<T> *tail = nullptr;

    int get_position(int num_array, int array_index)
    {
        return num_array * n + array_index;
    }

    void rebuild(const T &item, bool place_at_front)
    {
        std::vector<T> items;
        if (place_at_front)
        {
            items.push_back(item);
        }
        // collect all present elements in the data structure and set them as deleted
        if (head)
        {
            QueueNode<T> *QueueNode = head;
            QueueNode->deleted = true;
            items.push_back(QueueNode->data);
            while (QueueNode->next)
            {
                QueueNode = QueueNode->next;
                QueueNode->deleted = true;
                items.push_back(QueueNode->data);
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
#ifndef cdm_
#define cdm_

#include <cstddef>
#include <array>
#include <vector>
#include <stdexcept>

#include "hash.h"

template <typename T>
class CdmNode
{
public:
    T data;
    int *points_to;

    CdmNode()
    {
        points_to = nullptr;
    }
};

template <typename T, int num_elements, int n, int k>
class ConstantTimeCollection
{
public:
    ConstantTimeCollection()
    {
        for (int i = 0; i < k; ++i)
        {
            h[i].set_range(n);
        }
        arrays.fill(0);
    }

    void insert(const T &item)
    {
        if (contains(item))
        {
            return;
        }
        if (members > num_elements)
        {
            throw std::runtime_error("Constant Time Collection: too many elements inserted");
        }

        // find an empty spot for the element from the following options:
        // {A_1[h_1(item)], A_2[h_2(item)], .., A_k[h_k(item)]}
        for (int i = 0; i < k; ++i)
        {
            int position = get_position(i, h[i].hash(item));
            int *a = &(arrays[position]);
            CdmNode<T> b = elements[*a];

            // this spot is empty
            if (b.points_to != a || *a >= members)
            {
                arrays[position] = members;
                elements[members].points_to = &(arrays[position]);
                elements[members].data = item;
                members++;
                return;
            }
        }

        // didn't find an empty spot for the element => rebuild the whole data structure
        rebuild(item);
    }

    bool contains(const T &item) const
    {
        for (int i = 0; i < k; ++i)
        {
            const int position = get_position(i, h[i].hash(item));
            const int *a = &(arrays[position]);
            // somehow I can get a seg fault here
            // somehow there can be arbitrary elements in the variable arrays (large ones and negative ones)
            // this happens if elements overflows (more elements in the collection that it was intialized with)
            const CdmNode<T> b = elements[*a];

            if (b.points_to == a && b.data == item && *a < members)
            {
                return true;
            }
        }

        return false;
    }

    void reset()
    {
        members = 0;
    }

    bool empty() const
    {
        return !members;
    }

    int size() const
    {
        return members;
    }

private:
    int members = 0;
    std::array<CdmNode<T>, num_elements> elements;
    std::array<int, k * n> arrays;
    std::array<CarterWegmanHash<T>, k> h;

    int get_position(int num_array, int array_index) const
    {
        return num_array * n + array_index;
    }

    void rebuild(const T &item)
    {
        std::vector<T> items;
        items.reserve(members + 1);
        items.push_back(item);
        // collect all present elements in the data structure
        for (int i = 0; i < members; i++)
        {
            items.push_back(elements[i].data);
        }

        // sample new hash functions
        for (int i = 0; i < k; i++)
        {
            h[i].randomize_parameters();
        }
        reset();

        // insert all elements into the data structure again (with new seed), this may call rebuild again
        for (const T &elem : items)
        {
            insert(elem);
        }
    }
};

template <typename T, int num_elements, int n, int k>
class CycleDetectionMechanism
{
public:
    CycleDetectionMechanism()
    {
        collection = ConstantTimeCollection<T, num_elements, n, k>();
    }

    void insert(const T &item)
    {
        collection.insert(item);
    }

    // only returns true once lookup on the collection returned true at least one times
    bool contains(const T &item)
    {
        if (collection.contains(item))
        {
            if (duplicate)
            {
                return true;
            }
            duplicate = true;
        }
        return false;
    }

    void reset()
    {
        collection.reset();
        duplicate = false;
    }

    int size() const
    {
        return collection.size();
    }

private:
    ConstantTimeCollection<T, num_elements, n, k> collection;
    bool duplicate = false;
};

#endif
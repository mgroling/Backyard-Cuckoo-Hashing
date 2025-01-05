#ifndef simple_bin_
#define simple_bin_

#include <cstddef>
#include <array>
#include "hash.h"

template <typename T, int capacity>
class SimpleBin
{
public:
    SimpleBin()
    {
        elems.fill(T{});
        deleted.fill(true);
        num_elems = 0;
    }

    bool insert(const T &item)
    {
        for (int i = 0; i < capacity; ++i)
        {
            if (deleted[i])
            {
                elems[i] = item;
                deleted[i] = false;
                num_elems++;
                return true;
            }
        }
        return false;
    }

    bool remove(const T &item)
    {
        for (int i = 0; i < capacity; ++i)
        {
            if (!deleted[i] && elems[i] == item)
            {
                deleted[i] = true;
                num_elems--;
                return true;
            }
        }
        return false;
    }

    bool contains(const T &item) const
    {
        for (int i = 0; i < capacity; ++i)
        {
            if (!deleted[i] && elems[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    int size() const
    {
        return num_elems;
    }

    bool has_space() const
    {
        return num_elems < capacity;
    }

private:
    std::array<T, capacity> elems;
    std::array<bool, capacity> deleted;
    int num_elems;
};

template <typename T, int num_bins, int bin_capacity>
class SimpleBinCollection
{
public:
    SimpleBinCollection()
    {
        bins.fill(SimpleBin<T, bin_capacity>{});
        h.set_range(num_bins);
        _size = 0;
    }

    bool insert(const T &item)
    {
        SimpleBin<T, bin_capacity> &bin = bins[h.hash(item)];
        if (bin.has_space())
        {
            bin.insert(item);
            ++_size;
            return true;
        }
        return false;
    }

    bool remove(const T &item)
    {
        if (bins[h.hash(item)].remove(item))
        {
            _size--;
            return true;
        }
        return false;
    }

    bool contains(const T &item) const
    {
        return bins[h.hash(item)].contains(item);
    }

    int size() const
    {
        return _size;
    }

private:
    std::array<SimpleBin<T, bin_capacity>, num_bins> bins;
    TornadoHash<T> h;
    int _size;
};

#endif
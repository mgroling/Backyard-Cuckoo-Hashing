#include <cstddef>
#include <array>

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

    bool remove(const T &item)
    {
        for (int i = 0; i < capacity; ++i)
        {
            if (elems[i] == item)
            {
                deleted[i] = true;
                num_elems--;
                return true;
            }
        }
        return false;
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

    int size() const
    {
        return num_elems;
    }

private:
    std::array<T, capacity> elems;
    std::array<bool, capacity> deleted;
    int num_elems;
};
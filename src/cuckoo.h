#ifndef cuckoo
#define cuckoo

#include <vector>
#include <array>

template <typename T, size_t n>
class CuckooHashTable
{
public:
    std::array<T, n> t1;
    std::array<T, n> t2;
};

#endif
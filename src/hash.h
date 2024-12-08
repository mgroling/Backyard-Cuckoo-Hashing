#ifndef hash_
#define hash_

#include <array>
#include <cstdint>
#include <random>
#include "large_primes.h"

uint64_t sample_prime()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distr(0, num_primes - 1);
    return large_primes[distr(gen)];
}

// Abstract base class for pairwise independent hash functions (hash family)
template <typename T>
class PairwiseIndependentHashFunction
{
public:
    virtual void set_range(uint64_t m) = 0;

    virtual void randomize_parameters() = 0;

    virtual uint64_t hash(const T &item) const = 0;
};

// Specialization for uint64_t
template <>
class PairwiseIndependentHashFunction<uint64_t>
{
public:
    PairwiseIndependentHashFunction()
    {
        randomize_parameters();
    }

    void set_range(uint64_t m)
    {
        this->m = m;
    }

    void randomize_parameters()
    {
        p = sample_prime();
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint64_t hash(const uint64_t &item) const
    {
        return ((a * item + b) % p) % m;
    }

private:
    uint64_t a, b, p, m;
};

// Specialization for (y, b) pairs
template <>
class PairwiseIndependentHashFunction<std::pair<uint64_t, uint8_t>>
{
public:
    PairwiseIndependentHashFunction()
    {
        randomize_parameters();
    }

    void set_range(uint64_t m)
    {
        this->m = m;
    }

    void randomize_parameters()
    {
        p = sample_prime();
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint64_t hash(const std::pair<uint64_t, bool> &item) const
    {
        return ((a * (item.first + item.second * m) + b) % p) % m;
    }

private:
    uint64_t a, b, p, m;
};

#endif
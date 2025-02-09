#ifndef hash_
#define hash_

#include <array>
#include <cstdint>
#include <random>
#include "large_primes.h"

// Define a static random number generator with a fixed seed to be able to reproduce results.
static std::mt19937_64 gen(42);

uint64_t sample_prime()
{
    std::uniform_int_distribution<size_t> distr(0, num_primes - 1);
    return large_primes[distr(gen)];
}

// Abstract base class for pairwise independent hash functions (hash family)
template <typename T>
class CarterWegmanHash
{
public:
    virtual void set_range(uint64_t m) = 0;

    virtual void randomize_parameters() = 0;

    virtual uint32_t hash(const T &item) const = 0;
};

// Specialization for uint64_t
template <>
class CarterWegmanHash<uint64_t>
{
public:
    CarterWegmanHash()
    {
        randomize_parameters();
    }

    void set_range(uint32_t m)
    {
        this->m = m;
    }

    void randomize_parameters()
    {
        p = sample_prime();
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint32_t hash(const uint64_t &item) const
    {
        return ((a * item + b) % p) % m;
    }

private:
    uint32_t m;
    uint64_t a, b, p;
};

// Specialization for uint32_t
template <>
class CarterWegmanHash<uint32_t>
{
public:
    CarterWegmanHash()
    {
        randomize_parameters();
    }

    void set_range(uint32_t m)
    {
        this->m = m;
    }

    void randomize_parameters()
    {
        p = sample_prime();
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint32_t hash(const uint32_t &item) const
    {
        return ((a * item + b) % p) % m;
    }

private:
    uint32_t m;
    uint64_t a, b, p;
};

// Specialization for (y, b) pairs
template <>
class CarterWegmanHash<std::pair<uint32_t, bool>>
{
public:
    CarterWegmanHash()
    {
        randomize_parameters();
    }

    void set_range(uint32_t m)
    {
        this->m = m;
    }

    void randomize_parameters()
    {
        p = sample_prime();
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint32_t hash(const std::pair<uint32_t, bool> &item) const
    {
        return ((a * (item.first + item.second * m) + b) % p) % m;
    }

private:
    uint32_t m;
    uint64_t a, b, p;
};

// Abstract base class template
template <typename T>
class TornadoHash
{
public:
    virtual void set_range(uint64_t m) = 0;

    virtual void randomize_parameters() = 0;

    virtual uint64_t hash(const T &item) const = 0;
};

// Specialisation for 32 bit ints
template <>
class TornadoHash<uint32_t>
{
public:
    TornadoHash()
    {
        randomize_parameters();
    }

    void set_range(uint32_t m)
    {
        modulus = m;
    }

    void randomize_parameters()
    {
        std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

        for (uint64_t &elem : random_bits)
        {
            elem = dist(gen);
        }
    }

    uint32_t hash(const uint32_t &item) const
    {
        uint32_t x = item;
        uint64_t h = 0;
        uint8_t c;
        for (int i = 0; i < 3; ++i)
        {
            c = x;
            x >>= 8;
            h ^= random_bits[(i << 8) + c];
        }
        h ^= x;
        for (int i = 3; i < 8; ++i)
        {
            c = h;
            h >>= 8;
            h ^= random_bits[(i << 8) + c];
        }
        return ((uint32_t)h) % modulus;
    }

private:
    std::array<uint64_t, 8 * 256> random_bits;
    uint32_t modulus;
};

#endif
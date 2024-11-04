#ifndef hash_
#define hash_

#include <array>
#include <cstdint>
#include <random>

constexpr size_t num_primes = 10;
static const std::array<uint64_t, num_primes> large_primes = {
    96249602706718843UL,
    16195944769656221UL,
    61243980399666923UL,
    91165090352484931UL,
    70653361301029763UL,
    17239878924401069UL,
    26263798480984763UL,
    34752750108724387UL,
    78183116390846231UL,
    99471240469200793UL};

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
    virtual uint64_t hash(const T &item) = 0;
};

// Specialization for uint64_t
template <>
class PairwiseIndependentHashFunction<uint64_t>
{
public:
    PairwiseIndependentHashFunction()
    {
        p = sample_prime();
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    uint64_t hash(const uint64_t &item)
    {
        return ((a * item + b) % p);
    }

private:
    uint64_t a, b, p;
};

#endif

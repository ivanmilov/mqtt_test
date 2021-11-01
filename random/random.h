#pragma once

#include <bit>
#include <cstdint>
#include <limits>

class Rand
{
  public:
    Rand()
    {
        // pseudo random number for initial seed from current time
        time_t result = time(NULL);
        if (result != (time_t)(-1))
        {
            rng = xoroshiro128((intmax_t)result, (intmax_t)result);
        }
    }

    uint64_t next(uint64_t modulo = std::numeric_limits<uint64_t>::max()) { return rng.next() % modulo; }

  private:
    // https://prng.di.unimi.it/xoroshiro128plusplus.c
    // https://www.youtube.com/watch?v=xMdwK9p5qOw
    struct xoroshiro128
    {
        std::uint64_t s[2]{1, 1};

        xoroshiro128() = default;
        constexpr xoroshiro128(uint64_t s0, uint64_t s1) : s{s0, s1} {}

        // create a new random number generator with the state of the next two prng's from this copy
        constexpr xoroshiro128 fork() { return xoroshiro128{next(), next()}; }

        constexpr uint64_t next() noexcept
        {
            const auto s0 = s[0];
            auto s1 = s[1];
            const auto result = std::rotl(s0 + s1, 17) + s0;

            s1 ^= s0;
            s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21);
            s[1] = std::rotl(s1, 28);

            return result;
        }
    };

  private:
    xoroshiro128 rng;
};

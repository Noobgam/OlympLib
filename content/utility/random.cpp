#include <utility>

namespace olymplib {

    static std::size_t x=123456789, y=362436069, z=521288629;

    void Srand(std::size_t seed) {
        x=123456789, y=362436069, z=521288629;
        y ^= seed;
        y ^= (y << 16);
        z ^= seed;
        z ^= (z << 16);
    }

    std::size_t Rand() {
       std::size_t t;
       x ^= x << 16;
       x ^= x >> 5;
       x ^= x << 1;

       t = x;
       x = y;
       y = z;
       z = t ^ x ^ y;

       return z;
    }
}

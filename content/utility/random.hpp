
#ifndef OLYMPLIB_RANDOM
#define OLYMPLIB_RANDOM

namespace olymplib {

    static size_t x=123456789, y=362436069, z=521288629;

    void Srand(size_t seed) {
        x=123456789, y=362436069, z=521288629;
        y ^= seed;
        y ^= (y << 16);
        z ^= seed;
        z ^= (z << 16);
    }

    size_t Rand() {
       size_t t;
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

#endif

#include <utility>

#ifndef OLYMPLIB_GCD
#define OLYMPLIB_GCD

namespace olymplib {
	template <class T>
	T gcd(T a, T b) {
        while (a != 0 && b != 0) {
            if (a > b) a %= b;
            else b %= a;
        }
        return a + b;
    }

    template <class T>
    T binary_gcd(T a, T b) {
        size_t shift = 0;
        if (a == 0) return b;
        if (b == 0) return a;
        while (((a | b) & 1) == 0) {
            ++shift;
            a >>= 1;
            b >>= 1;
        }

        while ((b & 1) == 0) {
            b >>= 1;
        }
        do {
            while ((a & 1) == 0) {
                a >>= 1;
            }
            if (a > b) {
                swap(a, b);
            }
            b -= a;
        } while (b != 0);
        return a << shift;
    }
}

#endif

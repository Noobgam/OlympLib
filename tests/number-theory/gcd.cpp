#include "gtest/gtest.h"
#include "number-theory/gcd.hpp"
#include "utility/random.h"

using namespace olymplib;
using namespace std;

TEST(correctness, gcd) {
    EXPECT_EQ(2, gcd(2, 4));
    EXPECT_EQ(3, gcd(3, 9));
    EXPECT_EQ(4, gcd(4, 4));
    EXPECT_EQ(0, gcd(0, 0));
    EXPECT_EQ(4, gcd(0, 4));
}


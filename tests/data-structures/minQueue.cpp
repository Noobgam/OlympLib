#include "gtest/gtest.h"
#include "data-structures/minQueue.hpp"

using namespace olymplib;

TEST(correctness, pushpop) {
    MinQueue<int> a;
    a.push(1);
    EXPECT_EQ(a.getMin(), 1);
    a.push(2);
    EXPECT_EQ(a.getMin(), 1);
    a.push(2);
    EXPECT_EQ(a.getMin(), 1);
    a.pop();
    EXPECT_EQ(a.getMin(), 2);
    a.pop();
    EXPECT_EQ(a.getMin(), 2);
    a.push(1);
    EXPECT_EQ(a.getMin(), 1);
}

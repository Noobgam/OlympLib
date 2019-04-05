#include "gtest/gtest.h"
#include "data-structures/minQueue.hpp"
#include <random>
#include <set>
#include <queue>

using namespace olymplib;
using namespace std;

TEST(correctness, pushpop) {
    MinQueue<int> a;
    a.push(2);
    EXPECT_EQ(a.get_min(), 2);
    a.push(4);
    EXPECT_EQ(a.get_min(), 2);
    a.pop();
    EXPECT_EQ(a.get_min(), 4);
    a.push(3);
    EXPECT_EQ(a.get_min(), 3);
    a.pop();
    EXPECT_EQ(a.get_min(), 3);
    a.pop();
}

TEST(correctness, fuzzy_pushpop) {
    const int ITER = 1e4;
    for (int seed = 0; seed < 10; ++seed) {
        srand(seed);
        MinQueue<int> a;
        multiset<int> ss;
        queue <int> q;
        for (int i = 0; i < ITER; ++i) {
            int type = 0;
            if (ss.size() != 0) type = rand() % 2;
            if (type == 0) {
                int val = rand();
                ss.insert(val);
                q.push(val);
                a.push(val);
            } else {
                int front = q.front();
                a.pop();
                ss.erase(ss.lower_bound(front));
                q.pop();
            }
            if (ss.size() != 0) {
                EXPECT_EQ(*ss.begin(), a.get_min()) << " on iteration " << i << " seed " << seed;
            }
        }
    }
}

TEST(correctness, fuzzy_pushpop_reserve) {
    const int ITER = 1e4;
    for (int seed = 0; seed < 10; ++seed) {
        srand(seed);
        MinQueue<int> a(ITER / 2);
        multiset<int> ss;
        queue <int> q;
        for (int i = 0; i < ITER; ++i) {
            int type = 0;
            if (ss.size() != 0) type = rand() % 2;
            if (type == 0) {
                int val = rand();
                ss.insert(val);
                q.push(val);
                a.push(val);
            } else {
                int front = q.front();
                a.pop();
                ss.erase(ss.lower_bound(front));
                q.pop();
            }
            if (ss.size() != 0) {
                EXPECT_EQ(*ss.begin(), a.get_min()) << " on iteration " << i << " seed " << seed;
            }
        }
    }
}

const int STRESS_ITER = 1e8;
TEST(stress, fuzzy_pushpop) {
    srand(228);
    MinQueue<int> a;
    size_t sz = 0;
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (sz != 0) 
            type = rand() % 2;
        if (type == 0) {
            int val = rand();
            a.push(val);
            ++sz;
        } else {
            a.pop();
            --sz;
        }
    }
}

TEST(stress, fuzzy_pushpop_stl) {
    srand(228);
    size_t sz = 0;
    multiset<int> ss;
    queue <int> q;
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (ss.size() != 0) type = rand() % 2;
        if (type == 0) {
            int val = rand();
            ss.insert(val);
            q.push(val);
        } else {
            int front = q.front();
            ss.erase(ss.lower_bound(front));
            q.pop();
        }
    }
}

TEST(stress, fuzzy_pushpop_reserve) {
    srand(228);
    MinQueue<int> a(STRESS_ITER / 2);
    size_t sz = 0;
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (sz != 0) 
            type = rand() % 2;
        if (type == 0) {
            int val = rand();
            a.push(val);
            ++sz;
        } else {
            a.pop();
            --sz;
        }
    }
}



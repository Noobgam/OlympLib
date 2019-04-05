#include "gtest/gtest.h"
#include "data-structures/minQueue.hpp"
#include "utility/random.hpp"
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
        Srand(seed);
        MinQueue<int> a;
        multiset<int> ss;
        queue <int> q;
        for (int i = 0; i < ITER; ++i) {
            int type = 0;
            if (ss.size() != 0) type = Rand() % 2;
            if (type == 0) {
                int val = Rand();
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
        Srand(seed);
        MinQueue<int> a(ITER / 2);
        multiset<int> ss;
        queue <int> q;
        for (int i = 0; i < ITER; ++i) {
            int type = 0;
            if (ss.size() != 0) type = Rand() % 2;
            if (type == 0) {
                int val = Rand();
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

const int STRESS_ITER = 1e9;
vector <int> fuzzy_pushpop(int STRESS_ITER=::STRESS_ITER) {
    Srand(228);
    MinQueue<int> a;
    vector <int> res;
    res.reserve(STRESS_ITER);
    size_t sz = 0;
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (sz != 0) 
            type = Rand() % 2;
        if (type == 0) {
            int val = Rand();
            a.push(val);
            ++sz;
        } else {
            a.pop();
            --sz;
        }
        if (sz != 0) {
            res.push_back(a.get_min());
        }
    }
    return res;
}

vector<int> fuzzy_pushpop_stl(int STRESS_ITER=::STRESS_ITER) {
    Srand(228);
    size_t sz = 0;
    multiset<int> ss;
    queue <int> q;
    vector <int> res;
    res.reserve(STRESS_ITER);
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (!ss.empty()) type = Rand() % 2;
        if (type == 0) {
            int val = Rand();
            ss.insert(val);
            q.push(val);
        } else {
            int front = q.front();
            ss.erase(ss.lower_bound(front));
            q.pop();
        }
        if (!ss.empty()) {
            res.push_back(*ss.begin());                
        }
    }
    return res;
}

vector <int> fuzzy_pushpop_dualstack(int STRESS_ITER=::STRESS_ITER) {
    Srand(228);
    size_t sz = 0;
    vector <pair<int,int>> a, b;
    vector <int> res;
    res.reserve(STRESS_ITER);
    auto push_to_vector = [](vector<pair<int,int>>& dest, int val) {
        if (dest.empty()) {
            dest.push_back({val, val});
        } else {
            dest.push_back({val, min(dest.back().second, val)});
        }
    };
    auto get_min = [&a, &b]() { 
        if (!a.empty() && !b.empty()) {
            return min(a.back().second, b.back().second);
        }
        return a.empty() ? b.back().second : a.back().second;
    };
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (sz != 0) type = Rand() % 2;
        if (type == 0) {
            int val = Rand();
            push_to_vector(b, val);
            ++sz;
        } else {
            if (a.empty()) {
                while (!b.empty()) {
                    push_to_vector(a, b.back().first);
                    b.pop_back();
                }
                vector<pair<int,int>> Empty;
                b.swap(Empty);
            }
            a.pop_back();                
            --sz;
        }
        if (sz > 0) {
            res.push_back(get_min());                
        }
    }
    return res;
}

vector<int> fuzzy_pushpop_reserve(int STRESS_ITER=::STRESS_ITER) {
    Srand(228);
    MinQueue<int> a(STRESS_ITER / 2);
    size_t sz = 0;
    vector <int> res;
    res.reserve(STRESS_ITER);
    for (int i = 0; i < STRESS_ITER; ++i) {
        int type = 0;
        if (sz != 0) 
            type = Rand() % 2;
        if (type == 0) {
            int val = Rand();
            a.push(val);
            ++sz;
        } else {
            a.pop();
            --sz;
        }
        if (sz != 0) {
            res.push_back(a.get_min());
        }
    }
    return res;
}

TEST(correctness, stress_correctness) {
    const int IT = 100;
    auto v1 = fuzzy_pushpop(IT);
    auto v2 = fuzzy_pushpop_reserve(IT);
    auto v3 = fuzzy_pushpop_dualstack(IT);
    auto v4 = fuzzy_pushpop_stl(IT);
    EXPECT_EQ(v1, v2);
    EXPECT_EQ(v2, v3);
    EXPECT_EQ(v3, v4);
}

TEST(stress, stress_fuzzy) {
    auto v1 = fuzzy_pushpop();
}

TEST(stress, stress_fuzzy_reserve) {
    auto v2 = fuzzy_pushpop_reserve();
}

TEST(stress, stress_fuzzy_dualstack) {
    auto v3 = fuzzy_pushpop_dualstack();
}

//TEST(stress, stress_fuzzy_stl) {
//    auto v4 = fuzzy_pushpop_stl();
//}

#include "gtest/gtest.h"
#include "vector.h"


namespace alg {
    namespace {
        TEST(Vector, DefaultConstructor) {
            vector<float> v();
        }

        TEST(Vector, FillConstructor) {
            vector<int> v(54, 3);
            for (int i = 0; i < 54; i++)EXPECT_EQ(v[0], 3);
            for(const int& x : v)EXPECT_EQ(x,3);
        }
    }
}
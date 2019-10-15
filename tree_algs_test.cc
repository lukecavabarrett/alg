#include "tree_algs.h"
#include <numeric>
#include "gtest/gtest.h"


namespace alg {
namespace {
TEST(Cartesian, Build) {
    std::vector<int> v = {1, 1, 2, 4, 2};
    std::vector<binary_node<int> > vt;
    vt.reserve(v.size());
    for (const int &x : v)vt.emplace_back(x);
    auto root = make_cartesian_tree<std::vector<binary_node<int> >::iterator, std::greater<int> >(vt.begin(), vt.end());
    EXPECT_EQ(root, vt.data() + 3);
    EXPECT_EQ(root->right, vt.data() + 4);
    EXPECT_EQ(root->left, vt.data() + 2);
    EXPECT_EQ(root->left->left, vt.data());
    EXPECT_EQ(root->left->left->right, vt.data() + 1);
}

}
}
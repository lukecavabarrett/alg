#include <unordered_set>

#include "tree_list.h"

#include "gtest/gtest.h"

namespace alg {
namespace {

TEST(TreeList,Build){
        tree_list<float> tlf;
        tree_list<double> tld;
        tree_list<int> tli;
        tree_list<char *> tlpc;
}

    TEST(TreeList,OrderStatistic){
        tree_list<int,__gnu_pbds::tree_order_statistics_node_update> t;
        constexpr int n = 10000;
        for(int i=0;i<n;i++)t.insert_at(t.end(),i);
        {
            auto it = t.begin();
            for (int i = 0; i < n; i++,++it)EXPECT_EQ(t.find_by_order(i), it);
        }
    }

    TEST(TreeList,RandomAccess){
        tree_list<int,alg::random_access_policy> t;
        constexpr int n = 10000;
        for(int i=0;i<n;i++)t.insert_at(t.end(),i);

        for (int i = 0; i < n; i++)EXPECT_EQ(t[i], i);

    }

TEST(TreeList,MaxPrefix){
        tree_list<int, max_prefix_policy> t;
        for(const int& x : {300,-302,200})t.insert_at(t.end(),x);
        EXPECT_EQ(t.get_max_prefix(),300);
        auto it = t.begin();
        t.modify(++it,-199);
        EXPECT_EQ(t.get_max_prefix(),301);
/*
        tree_list<double,numeric_policy<double>::max_prefix_policy> tf;
        for(const double& x : {30,-100,100})tf.insert_at(tf.end(),x);
        EXPECT_EQ(tf.get_max_prefix(),30);
        auto itf = tf.begin();
        tf.modify(++itf,-99.99999999);
        EXPECT_GT(tf.get_max_prefix(),30);*/
}

}
}

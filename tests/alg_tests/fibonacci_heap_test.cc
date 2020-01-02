#include "gtest/gtest.h"
#include "fibonacci_heap.h"


namespace alg {
namespace {

TEST(FibonacciHeap, Sample) {
    fibonacci_set<int> fs;
    auto it1 = fs.push(4);
    EXPECT_EQ(fs.top(), it1);
    EXPECT_EQ(*fs.top(), 4);

    auto it2 = fs.push(3);
    EXPECT_EQ(fs.top(), it2);
    EXPECT_EQ(*fs.top(), 3);

    fs.decrease_key(it1, 2);
    EXPECT_EQ(*it1, 2);
    EXPECT_EQ(fs.top(), it1);
    EXPECT_EQ(*fs.top(), 2);

    EXPECT_EQ(fs.size(), 2);
    fs.pop();
    EXPECT_EQ(fs.size(), 1);

    fs.pop();
    EXPECT_EQ(fs.size(), 0);
    EXPECT_TRUE(fs.empty());
}

TEST(FibonacciHeap, Medium) {
    fibonacci_set<int> fs;
    for (int i = 0; i < 100; i++)fs.push(i);
    EXPECT_EQ(*fs.top(), 0);
    //fs.debug();
}

typedef unsigned long long int distance_t;

std::vector<distance_t> dijktsra_sssp(const std::vector<std::vector<std::pair<int, distance_t> > > &adj, int source) {
    int n = adj.size();
    std::vector<fibonacci_map<distance_t, int>::iterator> its(n);
    std::vector<distance_t> dst(n, std::numeric_limits<distance_t>::max());
    fibonacci_map<distance_t, int> fm;
    its[source] = fm.push(source, 0);
    while (!fm.empty()) {
        auto[t, u] = *fm.top();
        fm.pop();
        dst[u] = t;
        for (auto[v, d] : adj[u]) {
            if (its[v].uninitialized()) {
                its[v] = fm.push(t + d, v);
            } else if (t + d < its[v]->first) {
                fm.decrease_key(its[v], t + d);
            }
        }
    }
    return dst;
}

TEST(FibonacciHeap, Dijkstra) {
    EXPECT_EQ(dijktsra_sssp({
                                    {{1, 100}, {2, 10}},
                                    {{3, 5}},
                                    {{1, 10}},
                                    {}},
                            0),
              std::vector<distance_t>({0, 20, 10, 25}));
}

}
}

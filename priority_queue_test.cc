#include <unordered_set>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include "gtest/gtest.h"
#include "priority_queue.h"
#include <numeric>

namespace alg {
namespace {

template<typename T, class C>
void testPriorityQueueTC() {
    priority_queue<T, C> pq;
    pq.push(T());
    pq.push(T());
    T x;
    pq.push(std::move(x));
    pq.emplace();
    EXPECT_EQ(pq.size(), 4);
    EXPECT_FALSE(pq.empty());
    priority_queue<T, C> pq2(pq);
    EXPECT_FALSE(pq2.empty());
    pq2.clear();
    EXPECT_TRUE(pq2.empty());
    pq2.swap(pq);
    pq = pq2;
    EXPECT_EQ(pq2, pq);
}

TEST(PriorityQueue, UnderlyingContainers) {
    testPriorityQueueTC<int, alg::vector<int> >();
    testPriorityQueueTC<int, std::vector<int> >();
//testPriorityQueueTC<int,std::list <int> >(); // requires RAIt!
//testPriorityQueueTC<int,std::forward_list <int> >(); // requires size!
    testPriorityQueueTC<int, std::deque<int> >();
}

TEST(PriorityQueue, Comparer) {
    priority_queue<float, vector<float>, std::greater<> > pq;
    for (float i = 1.0; i < 20.0; i *= 1.3)pq.push(i);
    for (float i = 1.0; i < 20.0; i *= 1.3) {
        EXPECT_EQ(pq.top(), i);
        pq.pop();
    }
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueue, Performance) {
    constexpr int N = 1000000;
    vector<int> v(N);
    std::iota(v.begin(),v.end(),0);
    std::random_shuffle(v.begin(),v.end());
    priority_queue<int> pq;
    pq.push(v[0]);
    std::partial_sum(v.begin(),v.end(),v.begin(),[&pq](const int& a,const int& b){
        pq.push(b);
        EXPECT_EQ(pq.top(),std::max(a,b));
        return std::max(a,b);
    });
}


}
}
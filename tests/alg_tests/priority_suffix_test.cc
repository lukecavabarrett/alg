#include <unordered_set>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include "gtest/gtest.h"
#include "priority_suffix.h"
#include "vector.h"
#include <numeric>

namespace alg {
namespace {

template<class InputIt, class Compare = std::less<typename std::iterator_traits<InputIt>::value_type> >
size_t longest_decreasing_subsequence(InputIt first, InputIt last, Compare c = Compare()) {
    typedef typename std::iterator_traits<InputIt>::value_type value_type;
    priority_suffix<value_type, std::size_t, Compare > dp;
    for (; first != last; ++first) {
        auto it = dp.upper_bound(*first);
        std::size_t l = it == dp.end() ? 0 : it->second;
        dp.insert(*first, l + 1);
    }
    auto it = dp.begin();
    return it == dp.end() ? 0 : it->second;
}

template<class InputIt, class Compare = std::less<typename std::iterator_traits<InputIt>::value_type> >
size_t longest_nonincreasing_subsequence(InputIt first, InputIt last, Compare c = Compare()) {
    typedef typename std::iterator_traits<InputIt>::value_type value_type;
    priority_suffix<value_type, std::size_t, Compare > dp;
    for (; first != last; ++first) {
        auto it = dp.lower_bound(*first);
        std::size_t l = it == dp.end() ? 0 : it->second;
        dp.insert(*first, l + 1);
    }
    auto it = dp.begin();
    return it == dp.end() ? 0 : it->second;
}

TEST(PrioritySuffix, LongestIncreasingSubsequence) {
    vector<int> v = {23, 1, 1, 14, 14, 12, 12, 14, 23, 12, 23, 12};
    EXPECT_EQ(longest_decreasing_subsequence(v.begin(), v.end(),std::greater<int>()), 4);
    EXPECT_EQ(longest_nonincreasing_subsequence(v.begin(), v.end(),std::greater<int>()), 7);
}

TEST(PrioritySuffix,Generic){
    priority_suffix<float,int> ps;
    ps.insert(4.5,3);
    EXPECT_EQ(ps.lower_bound(0)->second,3);
    EXPECT_EQ(ps.upper_bound(4.5),ps.end());
}


}
}
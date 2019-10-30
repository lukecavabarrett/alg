#include "gtest/gtest.h"
#include "max_queue.h"
#include "vector.h"
#include <vector>

namespace alg {
namespace {

template <class T,class C = std::less<T> >
void cubic_test(const vector<T>& v, C comp = C()){
    for(int k=1;k<=v.size();k++){
        max_queue<T,C> mq(v.begin(),v.begin()+k);
        EXPECT_EQ(mq.size(),k);
        EXPECT_EQ(mq.top(),*std::max_element(v.begin(),v.begin()+k,comp));
        while(mq.end()<v.size()){
            mq.pop();
            mq.push(v[mq.end()]);
            EXPECT_EQ(mq.size(),k);
            EXPECT_EQ(mq.top(),*std::max_element(v.begin()+mq.begin(),v.begin()+mq.end(),comp));
        }
    }
}

TEST(MaxQueue,KeepTrack){
    cubic_test<int>({2,13,1,14,6,14,14,216,15});
    cubic_test<int,std::greater<int> >({2,13,1,14,6,14,14,216,15});
    cubic_test<int,std::greater_equal<int> >({2,13,1,14,6,14,14,216,15});
    cubic_test<int,std::less_equal<int> >({2,13,1,14,6,14,14,216,15});
}

}
}
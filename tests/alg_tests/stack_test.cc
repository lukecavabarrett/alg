#include <deque>
#include <list>
#include <forward_list>
#include "gtest/gtest.h"
#include "stack.h"


namespace alg {
namespace {

TEST(Stack,Build){
    stack<int,vector<int>> stk;
    stack<int,vector<int>> stk2(stk);
    stk2 = stk;
}

TEST(Stack,Emplace){
    stack<vector<char> > stk;
    constexpr char s[] = "Hello, World!";
    vector<char> a(s,s+strlen(s));

    stk.push(a);
    EXPECT_EQ(stk.size(),1);
    EXPECT_EQ(stk.top().size(),strlen(s));
    EXPECT_EQ(a.size(),strlen(s));

    stk.push(std::move(a));
    EXPECT_EQ(stk.size(),2);
    EXPECT_EQ(stk.top().size(),strlen(s));
    EXPECT_EQ(a.size(),0);

    stk.emplace(s,s+strlen(s));
    EXPECT_EQ(stk.size(),3);
    EXPECT_EQ(stk.top().size(),strlen(s));
}

template<typename T,class C>
void testStackTC(){
    stack<T,C> stk;
    stk.push(T());
    stk.push(T());
    T x;
    stk.push(std::move(x));
    stk.emplace();
    EXPECT_EQ(stk.size(),4);
    EXPECT_FALSE(stk.empty());
    stack<T,C> stk2(stk);
    EXPECT_FALSE(stk2.empty());
    stk2.clear();
    EXPECT_TRUE(stk2.empty());
    stk2.swap(stk);
    stk = stk2;
    EXPECT_EQ(stk2,stk);
}

TEST(Stack,UnderlyingContainers){
    testStackTC<int,alg::vector<int> >();
    testStackTC<int,std::vector<int> >();
    testStackTC<int,std::list <int> >();
    //testStackTC<int,std::forward_list <int> >(); has no size!
    testStackTC<int,std::deque<int> >();
}

}
}
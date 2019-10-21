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

}
}
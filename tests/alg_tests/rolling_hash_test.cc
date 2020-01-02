#include "rolling_hash.h"
#include "gtest/gtest.h"
#include "rolling_hash_multi.h"
//#include "aux/pretty_print.h"


namespace alg {
namespace {

typedef rolling_hash<char> string_hash;

TEST(RollingHash, Build) {
    EXPECT_EQ(string_hash::base_value, 257);
    std::string s = "Hello, World!";
    string_hash x(s.begin(), s.end());
    string_hash::hasher h;
}

TEST(RollingHash, SampleTest) {
    std::string s = "Hello, World!";
    string_hash x1(s);
    std::string_view s1(s);
    s1.remove_suffix(s1.size() - 6);
    std::string_view s2(s);
    s2.remove_prefix(6);
    string_hash x2 = string_hash(s1) + string_hash(s2);
    EXPECT_EQ(x1, x2);
}

TEST(RollingHashMulti, Build) {
    std::string s = "Hello, World!";
    rolling_hash_multi<char, 1> y(s);
    EXPECT_EQ(sizeof(rolling_hash_multi<char, 1>), sizeof(rolling_hash<char>));
    constexpr auto bv = rolling_hash_multi<char, 1>::base_value;
    EXPECT_EQ(bv, std::make_tuple(257LLU));
    rolling_hash_multi<char> x(s);
    EXPECT_EQ(sizeof(rolling_hash_multi<char>), sizeof(rolling_hash<char>) * 3);
    constexpr auto bv3 = rolling_hash_multi<char>::base_value;
    EXPECT_EQ(bv3, std::make_tuple(257LLU, 263LLU, 269LLU));
}

TEST(RollingHashMulti, SampleTest) {
    constexpr char s[] = "Hello, World!";
    std::string_view a(s, 6);
    std::string_view b(s + 6, 7);
    std::string_view x(s, 5);
    std::string_view y(s + 5, 8);

    //single multi hash
    typedef rolling_hash_multi<char, 1> sm_hash;
    sm_hash sm1(a);
    sm_hash sm2(x);
    EXPECT_NE(sm1, sm2);
    sm1.append(b);
    sm2.append(y);
    EXPECT_EQ(sm1, sm2);

    auto[sm1_a] = sm1.raw();
    auto[sm2_a] = sm2.raw();
    EXPECT_EQ(sm1_a, sm2_a);

    //triple hash
    typedef rolling_hash_multi<char> m_hash;
    m_hash m1(a);
    m_hash m2(x);
    EXPECT_NE(m1, m2);
    m1.append(b);
    m2.append(y);
    EXPECT_EQ(m1, m2);
    auto[m1_a, m1_b, m1_c] = m1.raw();
    auto[m2_a, m2_b, m2_c] = m2.raw();
    EXPECT_EQ(m1_a, m2_a);
    EXPECT_EQ(m1_b, m2_b);
    EXPECT_EQ(m1_c, m2_c);

    //std::cout << std::hex << m1.raw() << std::endl; need aux::pretty_print
}

}
}
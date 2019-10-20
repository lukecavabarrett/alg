#include "gtest/gtest.h"
#include "vector.h"


namespace alg {
namespace {
TEST(Vector, DefaultConstructor) {
    vector<float> v;
}

TEST(Vector, FillConstructor) {
    vector<int> v(size_t(54), 3);
    for (int i = 0; i < 54; i++)EXPECT_EQ(v[0], 3);
    for (const int &x : v)EXPECT_EQ(x, 3);
}

class dumbClass {
    static int id;
    int x, y;
public:
    dumbClass() : x(id++), y(-1) {
        std::cout << "allocated dC x: " << x << " y: " << y << std::endl;
    }

    dumbClass(int z) : x(id++), y(z) {
        std::cout << "allocated dC x: " << x << " y: " << y << std::endl;
    }

    dumbClass(const dumbClass &v) = delete;

    dumbClass(dumbClass &&v) : x(v.x), y(v.y) {}

    dumbClass &operator=(const dumbClass &v) = delete;

    dumbClass &operator=(dumbClass &&v) {
        x = v.x;
        y = v.y;
        return *this;
    }
};

int dumbClass::id = 0;

TEST(Vector, Allocation) {
    vector<dumbClass> v;
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(1);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(2);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(3);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(4);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(5);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(6);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(7);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(8);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(9);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(10);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(11);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(12);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
    v.emplace_back(13);
    std::cout << v.size() << "/" << v.capacity() << std::endl;
}

TEST(Vector,Move){
    vector<std::string> v;
    constexpr char s[] = "Hello, World!";
    std::string a(s,s+strlen(s));

    v.push_back(a);
    EXPECT_EQ(v.back().size(),strlen(s));
    EXPECT_EQ(a.size(),strlen(s));
    EXPECT_EQ(v.size(),1);
    EXPECT_GE(v.capacity(),1);
    EXPECT_EQ(v.back(),a);
    std::cout << a << std::endl;

    v.push_back(a);

    EXPECT_EQ(v.back().size(),strlen(s));
    EXPECT_EQ(a.size(),strlen(s));
    v.push_back(std::move(a));
    EXPECT_EQ(v.back().size(),strlen(s));
    // a might be unvalidated now
}
}
}
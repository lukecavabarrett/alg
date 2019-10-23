#include <unordered_set>
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

TEST(Vector, Move) {
    vector<std::string> v;
    constexpr char s[] = "Hello, World!";
    std::string a(s, s + strlen(s));

    v.push_back(a);
    EXPECT_EQ(v.back().size(), strlen(s));
    EXPECT_EQ(a.size(), strlen(s));
    EXPECT_EQ(v.size(), 1);
    EXPECT_GE(v.capacity(), 1);
    EXPECT_EQ(v.back(), a);

    v.push_back(a);
    EXPECT_EQ(v.back().size(), strlen(s));
    EXPECT_EQ(a.size(), strlen(s));

    v.push_back(std::move(a));
    EXPECT_EQ(v.back().size(), strlen(s));
    EXPECT_EQ(a.size(), 0);
}

class TestObject {
    static int n_id;
    static std::unordered_set<int> active_ids;
    int id;
public:
    TestObject() : id(n_id++) {
        EXPECT_TRUE(active_ids.insert(id).second);
    };

    TestObject(const TestObject &t) : id(n_id++) {
        EXPECT_TRUE(active_ids.insert(id).second);
    };

    TestObject(TestObject &&t) : id(n_id++) {
        EXPECT_TRUE(active_ids.insert(id).second);
    };

    ~TestObject() {
        EXPECT_EQ(active_ids.erase(id), 1);
    }

    TestObject& operator=(const TestObject& o){return *this;}
    TestObject& operator=(TestObject&& o){return *this;}

    static int AllocatedObjects() {
        return active_ids.size();
    }
};

int TestObject::n_id = 0;
std::unordered_set<int> TestObject::active_ids;

TEST(Vector, AllocationAndDestruction) {
    {
        vector<TestObject> v;
        EXPECT_EQ(TestObject::AllocatedObjects(), 0);
        for (int i = 0; i < 100; i++) {
            v.emplace_back();
            EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
        }
        for (int i = 0; i < 50; i++) {
            v.pop_back();
            EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
        }
        // ~vector();
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, AllocationAndClear) {
    {
        vector<TestObject> v;
        EXPECT_EQ(TestObject::AllocatedObjects(), 0);
        for (int i = 0; i < 100; i++) {
            v.push_back(TestObject());
            EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
        }
        for (int i = 0; i < 50; i++) {
            v.pop_back();
            EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
        }
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), 0);
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, DefaultInitializationAndDestroy) {
    {
        vector<TestObject> v(100);
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), 0);
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, CopyInitializationAndClear) {
    {
        TestObject to;
        vector<TestObject> v(100, to);
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size() + 1);
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), 1);
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, CopyConstructorAndClearAndDestroy) {

    vector<TestObject> v;
    for (int i = 0; i < 100; i++) {
        v.push_back(TestObject());
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
    }
    {
        vector<TestObject> vc(v);
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size() + vc.size());
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), vc.size());
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, MoveConstructorAndClearAndDestroy) {
    vector<TestObject> v;
    for (int i = 0; i < 100; i++) {
        v.push_back(TestObject());
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size());
    }
    {
        vector<TestObject> vm(std::move(v));
        EXPECT_EQ(v.size(),0);
        EXPECT_EQ(TestObject::AllocatedObjects(), vm.size());
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), vm.size());
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, AssignFromBigger) {
    vector<TestObject> v(100);
    {
        vector<TestObject> va(30);
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size() + va.size());
        va = v;

        EXPECT_EQ(TestObject::AllocatedObjects(), 2*v.size());
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), va.size());
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, AssignFromSmaller) {
    vector<TestObject> v(30);
    {
        vector<TestObject> va(100);
        EXPECT_EQ(TestObject::AllocatedObjects(), v.size() + va.size());
        va = v;

        EXPECT_EQ(TestObject::AllocatedObjects(), 2*v.size());
        v.clear();
        EXPECT_EQ(TestObject::AllocatedObjects(), va.size());
    }
    EXPECT_EQ(TestObject::AllocatedObjects(), 0);
}

TEST(Vector, Comparison) {
    vector<float> a = {213.44, 24.111, 31.09};
    vector<float> b = {213.44, 24.111, 31.09};
    EXPECT_EQ(a, b);
    b[1]++;
    EXPECT_NE(a, b);
    b[1] = a[1];
    EXPECT_EQ(a, b);
    b.pop_back();
    EXPECT_NE(a, b);
}

}
}
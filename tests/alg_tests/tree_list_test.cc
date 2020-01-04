#include <unordered_set>

#include "tree_list.h"

#include "gtest/gtest.h"


namespace alg {
    template<class T>
    constexpr
    std::string_view
    type_name() {
        using namespace std;
#ifdef __clang__
        string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
        string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
        return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
        return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
        string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
    }
}

namespace alg {
    namespace {

        TEST(TreeList, Build) {
            tree_list<float> tlf;
            tree_list<double> tld;
            tree_list<int> tli;
            tree_list<char *> tlpc;
        }

    TEST(TreeList,OrderStatistic){
        tree_list<int,__gnu_pbds::tree_order_statistics_node_update> t;
        constexpr int n = 10000;
        for (int i = 0; i < n; i++)t.insert(t.end(), i);
        {
            auto it = t.begin();
            for (int i = 0; i < n; i++,++it)EXPECT_EQ(t.find_by_order(i), it);
        }
    }

    TEST(TreeList, RandomAccess) {
        tree_list<int, alg::random_access_policy> t;
        constexpr int n = 10000;
        for (int i = 0; i < n; i++)t.insert(t.end(), i);

        for (int i = 0; i < n; i++)EXPECT_EQ(t[i], i);

    }

    TEST(TreeList, MaxPrefix) {
        tree_list<int, max_prefix_policy> t;
        for (const int &x : {300, -302, 200})t.insert(t.end(), x);
        EXPECT_EQ(t.get_max_prefix(), 300);
        auto it = t.begin();
        t.modify(++it, -199);
        EXPECT_EQ(t.get_max_prefix(), 301);

        tree_list<double, max_prefix_policy> tf;
        for (const double &x : {30, -100, 100})tf.insert(tf.end(), x);
        EXPECT_EQ(tf.get_max_prefix(), 30);
        auto itf = tf.begin();
        tf.modify(++itf, -99.99999999);
        EXPECT_GT(tf.get_max_prefix(), 30);
    }

    TEST(TreeList, Types) {
        typedef tree_list<int, __gnu_pbds::tree_order_statistics_node_update> tli;
        static_assert(std::is_same_v<tli::const_reference, const int &>);
        static_assert(std::is_same_v<tli::key_const_reference, const int &>);
        //std::cout << type_name<tli::const_reference>() << std::endl;
        //std::cout << type_name<tli::key_const_reference>() << std::endl;
    }

    TEST(TreeList, Insertion) {
        tree_list<int> tl;
        tl.insert(tl.end(), 1);
    }

    template<class T>
    struct dumb_allocator {
        static int n_al;
        const int id;
        const double useless;
    public:
        dumb_allocator() : id(++n_al), useless(M_PI) {
            std::cout << "instantiated dumb_allocator<" << type_name<T>() << "> " << id << std::endl;
        }

        template<class U>
        dumb_allocator(const dumb_allocator<U> &o) : id(++n_al), useless(M_E) {
            std::cout << "instantiated dumb_allocator<" << type_name<T>() << "> " << id
                      << " as a copy from dumb_allocator<" << type_name<U>() << "> " << o.id << std::endl;
        }

        ~dumb_allocator() {}

        T *allocate(std::size_t n) {
            return (T *) malloc(n * sizeof(T));
        }

        void deallocate(T *p, std::size_t n) {
            free((void *) p);
        }

        typedef T value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
    };

    template<class T> int dumb_allocator<T>::n_al = 0;

    TEST(TreeList, SetNodeHandlers) {
        std::map<int, int> m1;
        m1[3] = 4;
        std::map<int, int>::node_type n = m1.extract(m1.begin());
        //   std::cout << sizeof(std::map<int,int>::iterator) << std::endl;
        //    std::cout << sizeof(std::map<int,int>::allocator_type) << std::endl;
        //    std::cout << sizeof(std::map<int,int>) << std::endl;


        typedef std::map<int, int, std::less<int>, dumb_allocator<std::pair<const int, int> > > dumb_map;
        dumb_map m2 = {{1, 1},
                       {2, 2},
                       {3, 3}};
        dumb_map m3 = {{4, 4},
                       {5, 5},
                       {6, 7}};;

        m2.insert(m3.extract(m3.begin()));
        dumb_map::node_type n2 = m2.extract(m2.begin());
        EXPECT_FALSE(n2.empty());
        EXPECT_TRUE(n2);
        EXPECT_EQ(n2.key(), 1);
        EXPECT_EQ(n2.mapped(), 1);
        //std::cout << " n2 has allocator " << n2.get_allocator().id << std::endl;
        std::map<int, int> m4;

        std::cout << sizeof(std::map<int, int>::node_type) << std::endl;
        std::cout << sizeof(dumb_map::node_type) << std::endl;

    }

    template<typename T>
    class checked {
    public:
        static std::size_t n_instances;

        //template<typename ...Args>
        //explicit checked(Args&&... args) : inner(std::forward<Args>(args)...) {++n_instances;fprintf(stderr,"created by args %d\n",inner);}
        explicit checked(const T &i) : inner(i) {
            ++n_instances;
            fprintf(stdout, "created by Tconst_ref %d\n", inner);
        }

        explicit checked(T &&i) : inner(std::move(i)) {
            ++n_instances;
            fprintf(stdout, "created by Tr_ref %d\n", inner);
        }

        explicit checked(const checked &i) : inner(std::move(i.inner)) {
            ++n_instances;
            fprintf(stdout, "created by const_ref %d\n", inner);
        }

        explicit checked(checked &&i) : inner(i.inner) {
            ++n_instances;
            fprintf(stdout, "created by r_ref %d\n", inner);
        }

        checked &operator=(const checked &i) {
            inner = i.inner;
            return *this;
        }

        checked &operator=(checked &&i) {
            inner = std::move(i.inner);
            return *this;
        }

        ~checked() {
            fprintf(stdout, "destroyed %d\n", inner);
            inner = -1;
            --n_instances;
        }

        const T &get_inner() const { return inner; }

    private:
        T inner;
    };

    template<typename T>
    std::size_t checked<T>::n_instances = 0;

    TEST(TreeList, NodeHandlers) {
        typedef checked<int> cint;
        EXPECT_EQ(cint::n_instances, 0);
        {
            tree_list<cint, random_access_policy> t1, t2;
            for (int i = 0; i < 5; i++)t1.push_back(cint(i));
            for (int i = 5; i < 10; i++)t1.emplace_back(i);
            EXPECT_EQ(cint::n_instances, 10);
            EXPECT_EQ((*t1.begin()).get_inner(), 0);
            //1. Direct
            fprintf(stdout, "1. Direct\n");
            t2.push_back(t1.extract(t1.begin()));
            //2. Passing by nh
            fprintf(stdout, "2. Passing by nh\n");
            auto nh = t1.extract(t1.begin());
            t2.push_back(std::move(nh));
            //3. discarding result
            fprintf(stdout, "3. discarding result\n");
            t1.extract(t1.begin());
            //4. reassigning nh
            fprintf(stdout, "4. reassigning nh\n");
            nh = t1.extract(t1.begin());
            //5. reassigning nh again
            fprintf(stdout, "5. reassigning nh again\n");
            nh = t1.extract(t1.begin());
            //6. swapping nh
            fprintf(stdout, "6. swapping nh\n");
            auto nh2 = t1.extract(t1.begin());
            nh.swap(nh2);
            //N. Holding it
            fprintf(stdout, "Last. Holding it until out-of-scope\n");
            auto nh_last = t1.extract(t1.begin());
        } // Trigger destructors
            EXPECT_EQ(cint::n_instances, 0);
        }

        TEST(TreeList, RangeBuild) {
            typedef alg::tree_list<int, max_prefix_policy> tree;
            tree t0 = {};
            tree t1 = {1};
            tree t2 = {1, 2};
            tree t3 = {1, 2, 3};
            tree t_large(1000000, 42);
            std::cout << "expect 42,000,000" << std::endl;
            std::cout << t_large.get_max_prefix() << std::endl;
            for (auto it = t_large.begin(); it != t_large.end(); ++it) {
                t_large.modify(it, (rand() % 100) - 40);
            }
            std::cout << t_large.get_max_prefix() << std::endl;
        }

        namespace test {

            constexpr std::size_t TIME_ZERO = 0, NULL_ID = -1, UNINITIALIZED_ID = -2;
            std::size_t gl_clock = TIME_ZERO, gl_n_calls = 0, gl_n_wasted_calls = 0;
            struct metadata_struct {
                std::size_t id = UNINITIALIZED_ID, last_updated = TIME_ZERO, last_updated_true = TIME_ZERO, last_left_id = UNINITIALIZED_ID, last_right_id = UNINITIALIZED_ID;
            };

            template<typename Node_CItr, typename Node_Itr, typename Cmp_Fn, typename _Alloc>
            class observer_policy : private __gnu_pbds::detail::branch_policy<Node_CItr, Node_Itr, _Alloc> {
            private:
                typedef __gnu_pbds::detail::branch_policy<Node_CItr, Node_Itr, _Alloc> base_type;
            public:

                typedef typename base_type::key_type key_type;
                typedef typename base_type::key_const_reference key_const_reference;

                typedef Node_CItr node_const_iterator;
                typedef Node_Itr node_iterator;
                typedef typename node_iterator::value_type iterator;
                typedef typename std::remove_pointer_t<iterator> value_type;
                typedef metadata_struct metadata_type;
                typedef metadata_type &metadata_reference;

                void operator()(Node_Itr nd_it, Node_CItr end_nd_it) {
                    ++gl_clock;
                    metadata_reference md = const_cast<metadata_reference >(nd_it.get_metadata());
                    node_iterator l_it = nd_it.get_l_child();
                    node_iterator r_it = nd_it.get_r_child();


                    std::size_t l_id = l_it != end_nd_it ? l_it.get_metadata().id : NULL_ID;
                    std::size_t r_id = r_it != end_nd_it ? r_it.get_metadata().id : NULL_ID;
                    std::size_t l_upd = l_it != end_nd_it ? l_it.get_metadata().last_updated_true : TIME_ZERO;
                    std::size_t r_upd = r_it != end_nd_it ? r_it.get_metadata().last_updated_true : TIME_ZERO;

                    bool req = false;
                    req |= md.last_left_id != l_id;
                    req |= md.last_right_id != r_id;
                    req |= md.last_updated < std::max(l_upd, r_upd);
                    ++gl_n_calls;
                    if (!req)++gl_n_wasted_calls;

                    if (md.id == UNINITIALIZED_ID)md.id = gl_clock;
                    md.last_updated = gl_clock;
                    if (req)md.last_updated_true = gl_clock;
                    md.last_left_id = l_id;
                    md.last_right_id = r_id;
                }

                virtual Node_CItr
                node_begin() const = 0;

                virtual Node_CItr
                node_end() const = 0;
            };

        }

        TEST(TreeList, MetadataUpdates) {
            alg::tree_list<int, test::observer_policy> t;
            constexpr int n_test = 10000;
            for (int i = 0; i < n_test; ++i)t.push_back(i);

            //test::gl_n_wasted_calls=test::gl_n_calls=0;

            t.push_back(n_test);

            std::cout << " --- report ---" << std::endl;
            std::cout << " n_calls: " << test::gl_n_calls << std::endl;
            std::cout << " n_wasted_calls: " << test::gl_n_wasted_calls << std::endl;
            std::cout << " efficiency: " << 100 * double(test::gl_n_calls - test::gl_n_wasted_calls) / test::gl_n_calls
                      << "%" << std::endl;
        }


        TEST(TreeList, Miscellanea) {
            alg::tree_list<int, __gnu_pbds::tree_order_statistics_node_update> t;
            std::vector<int> data = {4, 5, 3, 6, 2, 7};
            for (const int &x : data) {
                std::cout << "inserting " << x << std::endl;
                t.insert(t.end(), x);
            }
            //t.copy_from_range(data.begin(),data.end());
        t.erase(t.begin());
        t.erase(t.rbegin());
        EXPECT_EQ(*t.find_by_order(3), 2);
        t.insert(t.find_by_order(3), 45);


        t.insert(t.begin(), 123);

        t.erase(t.find_by_order(t.size() - 3));

        alg::tree_list<int, __gnu_pbds::tree_order_statistics_node_update> t2;
        for (const int &x : data) {
            t2.insert(t2.end(), x);
        }

        t.join(t2);


        auto it = t.find_by_order(5);

        t.split_at_node(it.m_p_nd, t2);


        const alg::tree_list<int, __gnu_pbds::tree_order_statistics_node_update> &tref = t;

        alg::tree_list<int, __gnu_pbds::tree_order_statistics_node_update>::iterator imp = tref.find_by_order(3);
        t.insert(imp, 34);

        t.modify(imp, 777);

        alg::tree_list<int, max_prefix_policy> mp;
        for (const int &x : {300, -301, 300})mp.insert(mp.end(), x);
        auto itmp = mp.begin();
        ++itmp;
        mp.modify(itmp, -1000);
        EXPECT_EQ(mp.get_max_prefix(), 300);
        mp.modify(itmp, -290);
        EXPECT_EQ(mp.get_max_prefix(), 310);
    }

}
}

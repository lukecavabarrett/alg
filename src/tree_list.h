#ifndef TREES_TREE_LIST_H
#define TREES_TREE_LIST_H

namespace alg::detail {
    template<class T = void>
    struct null_comparer {
        typedef bool result_type;
        typedef T first_argument_type;
        typedef T second_argument_type;
        // constexpr bool operator()(const T &lhs, const T &rhs) const {}
    };
}

#include <ext/pb_ds/assoc_container.hpp>


namespace __gnu_pbds::detail {
    template<typename Key,
            typename Mapped,
            typename Cmp_Fn,
            typename Node_And_It_Traits,
            typename _Alloc>
    class rb_tree_list
            : public bin_search_tree_set<Key, __gnu_pbds::null_type, alg::detail::null_comparer<Key>, Node_And_It_Traits, _Alloc> {
    private:
        typedef bin_search_tree_set<Key, __gnu_pbds::null_type, alg::detail::null_comparer<Key>, Node_And_It_Traits, _Alloc> base_type;
        typedef typename base_type::node_pointer node_pointer;
        typedef typename base_type::node node;
        typedef typename base_type::node_allocator node_allocator;
    public:

        typedef rb_tree_tag container_category;
        typedef Cmp_Fn cmp_fn;
        typedef _Alloc allocator_type;
        typedef typename _Alloc::size_type size_type;
        typedef typename _Alloc::difference_type difference_type;
        typedef typename base_type::key_type key_type;
        typedef typename base_type::key_pointer key_pointer;
        typedef typename base_type::key_const_pointer key_const_pointer;
            typedef typename base_type::key_reference key_reference;
            typedef typename base_type::key_const_reference key_const_reference;
            typedef typename base_type::mapped_type mapped_type;
            typedef typename base_type::mapped_pointer mapped_pointer;
            typedef typename base_type::mapped_const_pointer mapped_const_pointer;
            typedef typename base_type::mapped_reference mapped_reference;
            typedef typename base_type::mapped_const_reference mapped_const_reference;
            typedef typename base_type::value_type value_type;
            typedef typename base_type::pointer pointer;
            typedef typename base_type::const_pointer const_pointer;
        typedef typename base_type::reference reference;
        typedef typename base_type::const_reference const_reference;
        typedef typename base_type::point_iterator point_iterator;
        typedef typename base_type::const_iterator point_const_iterator;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;
        typedef typename base_type::reverse_iterator reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::node_update node_update;

        class node_type {
        private:
            friend class rb_tree_list;

            //constructor
            node_type(node_pointer p, const node_allocator &a) : m_ptr(p), n_all(a) {}

        public:
            node_type() : m_ptr(nullptr) {}

            node_type(node_type &&o) noexcept : m_ptr(o.m_ptr), n_all(o.n_all) { o.m_ptr = nullptr; }

            void swap(node_type &o) {
                std::swap(m_ptr, o.m_ptr);
                std::swap(n_all, o.n_all);
            }

            node_type &operator=(node_type &&o) noexcept {
                swap(o);
                return *this;
            } //&& or &?
            key_reference key() { return m_ptr->m_p_value; }

            [[nodiscard]] bool empty() const { return m_ptr == nullptr; }

            explicit operator bool() const { return m_ptr != nullptr; }

            //destructor
            ~node_type() {
                if (m_ptr) {
                    m_ptr->~node();
                    n_all.deallocate(m_ptr, 1);
                }
            }

        private:
            node_pointer m_ptr;
            node_allocator n_all;
        };

    private:
        // node creation
        inline node_pointer __make_node_copy_constructor(const_reference);

        inline node_pointer __make_node_move_constructor(key_type &&);

        template<class... Args>
        inline node_pointer __make_node_custom_constructor(Args &&... args);

        inline node_pointer __make_node_node_handler(node_type &&nh);

        // base_type node insertion
        inline void __base_type__insert_imp_empty(node_pointer);

        inline void __base_type__insert_leaf(node_pointer, node_pointer, bool);

        inline void __base_type__insert_leaf_at(node_pointer, node_pointer);

        // node insertion
        inline point_iterator __insert(node_pointer, node_pointer);

        inline point_iterator __insert_back(node_pointer);

        inline point_iterator __insert_front(node_pointer);

        // node erasion
        inline void __erase_destroy(node_pointer);

        inline void __erase_destroy_front();

        inline void __erase_destroy_back();

        inline node_pointer __erase_get_ptr(node_pointer);

        inline node_pointer __erase_get_ptr_front();

        inline node_pointer __erase_get_ptr_back();

        inline point_iterator __erase_destroy_get_next(point_iterator);

        inline point_iterator __erase_destroy_get_prev(point_iterator);

    public:

        //constructors

        rb_tree_list();

        //rb_tree_list(const Cmp_Fn &);

        //rb_tree_list(const Cmp_Fn &, const node_update &);

        rb_tree_list(const rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

        void
        swap(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

        template<typename It>
        void
        copy_from_range(It, It);

        //inline std::pair<point_iterator, bool>            insert(const_reference);


        //insertors

        iterator insert(iterator it, const_reference cr) {
            return __insert(__make_node_copy_constructor(cr), it.m_p_nd);
        }

        iterator push_front(const_reference cr) { return __insert_front(__make_node_copy_constructor(cr)); }

        iterator push_back(const_reference cr) { return __insert_back(__make_node_copy_constructor(cr)); }

        iterator insert(iterator it, key_type &&rr) {
            return __insert(__make_node_move_constructor(std::move(rr)), it.m_p_nd);
        }

        iterator push_front(key_type &&rr) { return __insert_front(__make_node_move_constructor(std::move(rr))); }

        iterator push_back(key_type &&rr) { return __insert_back(__make_node_move_constructor(std::move(rr))); }

        iterator insert(iterator it, node_type &&nh) {
            return __insert(__make_node_node_handler(std::move(nh)), it.m_p_nd);
        }

        iterator push_front(node_type &&nh) { return __insert_front(__make_node_node_handler(std::move(nh))); }

        iterator push_back(node_type &&nh) { return __insert_back(__make_node_node_handler(std::move(nh))); }

        template<class... Args>
        iterator emplace(iterator it, Args &&... args) {
            return __insert(__make_node_custom_constructor(std::forward<Args>(args)...), it.m_p_nd);
        }

        template<class... Args>
        iterator emplace_front(Args &&... args) {
            return __insert_front(__make_node_custom_constructor(std::forward<Args>(args)...));
        }

        template<class... Args>
        iterator emplace_back(Args &&... args) {
            return __insert_back(__make_node_custom_constructor(std::forward<Args>(args)...));
        }

        //erasers

        iterator erase(iterator it) override { return __erase_destroy_get_next(it); }

        reverse_iterator erase(reverse_iterator it) override { return __erase_destroy_get_prev(it); }

        void pop_back() { __erase_destroy_front(); }

        void pop_front() { __erase_destroy_back(); }

        node_type extract(iterator it) {
            return node_type(__erase_get_ptr(it.m_p_nd), base_type::s_node_allocator);
        }

        node_type extract_front() { return node_type(__erase_get_ptr_front(), base_type::s_node_allocator); }

        node_type extract_back() { return node_type(__erase_get_ptr_back(), base_type::s_node_allocator); }


        template<typename Pred>
        inline size_type
        erase_if(Pred);

        void
        join(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

        void
            split(key_const_reference, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

        private:
        inline static bool
        is_effectively_black(node_pointer);

            void
            initialize();

            void
            insert_fixup(node_pointer);

            void
            erase_node(node_pointer);

            void
            remove_node(node_pointer);

            void
            remove_fixup(node_pointer, node_pointer);

            void
            split_imp(node_pointer, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

            inline node_pointer
            split_min();

            std::pair<node_pointer, node_pointer>
            split_min_imp();

            void
            join_imp(node_pointer, node_pointer);

            std::pair<node_pointer, node_pointer>
            find_join_pos_right(node_pointer, size_type, size_type);

            std::pair<node_pointer, node_pointer>
            find_join_pos_left(node_pointer, size_type, size_type);

            inline size_type
            black_height(node_pointer);

        public:
            void
            split_at_node(node_pointer, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

            void
            modify(point_iterator, const_reference);
        };

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        template<typename It>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        copy_from_range(It first_it, It last_it) {
            while (first_it != last_it)
                insert(*(first_it++));
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        rb_tree_list() {
            initialize();

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        rb_tree_list(const Cmp_Fn &r_cmp_fn) :
                base_type(r_cmp_fn) {
            initialize();

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        rb_tree_list(const Cmp_Fn &r_cmp_fn, const node_update &r_node_update) :
                base_type(r_cmp_fn, r_node_update) {
            initialize();

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        rb_tree_list(const rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &other) :
                base_type(other) {
            initialize();

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        swap(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &other) {

            base_type::swap(other);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        initialize() { base_type::m_p_head->m_red = true; }


    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __base_type__insert_leaf_at(node_pointer p_new_nd, node_pointer p_nd) {
        if (base_type::m_size == 0) {
            __base_type__insert_imp_empty(p_new_nd);
        } else if (p_nd == base_type::m_p_head) {
            __base_type__insert_leaf(p_new_nd, base_type::m_p_head->m_p_right, false);
        } else if (p_nd->m_p_left == nullptr) {
            __base_type__insert_leaf(p_new_nd, p_nd, true);
        } else {
            p_nd = p_nd->m_p_left;
            while (p_nd->m_p_right != nullptr)p_nd = p_nd->m_p_right;
            __base_type__insert_leaf(p_new_nd, p_nd, false);
        }
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __make_node_copy_constructor(const_reference r_val) {
        node_pointer p_new_nd = base_type::s_node_allocator.allocate(1);

        new(const_cast<void * >(static_cast<const void * >(&p_new_nd->m_value)))
                typename base_type::node::value_type(r_val);

        p_new_nd->m_p_left = p_new_nd->m_p_right = 0;
        return p_new_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __make_node_move_constructor(key_type &&rr_val) {
        node_pointer p_new_nd = base_type::s_node_allocator.allocate(1);

        new(const_cast<void * >(static_cast<const void * >(&p_new_nd->m_value)))
                typename base_type::node::value_type(rr_val);

        p_new_nd->m_p_left = p_new_nd->m_p_right = 0;
        return p_new_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    template<class... Args>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __make_node_custom_constructor(Args &&... args) {
        node_pointer p_new_nd = base_type::s_node_allocator.allocate(1);

        new(const_cast<void * >(static_cast<const void * >(&p_new_nd->m_value)))
                typename base_type::node::value_type(std::forward<Args>(args)...);

        p_new_nd->m_p_left = p_new_nd->m_p_right = 0;
        return p_new_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __make_node_node_handler(node_type &&nh) {
        node_pointer p_new_nd = nh.m_ptr;
        nh.m_ptr = nullptr;

        p_new_nd->m_p_left = p_new_nd->m_p_right = 0;
        return p_new_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __base_type__insert_imp_empty(node_pointer p_new_node) {
        ++base_type::m_size;
        base_type::m_p_head->m_p_left = base_type::m_p_head->m_p_right =
        base_type::m_p_head->m_p_parent = p_new_node;

        p_new_node->m_p_parent = base_type::m_p_head;
        p_new_node->m_p_left = p_new_node->m_p_right = 0;
        base_type::update_to_top(base_type::m_p_head->m_p_parent, (node_update *) this);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __base_type__insert_leaf(node_pointer p_new_nd, node_pointer p_nd, bool left_nd) {
        ++base_type::m_size;
        if (left_nd) {
            p_nd->m_p_left = p_new_nd;
            if (base_type::m_p_head->m_p_left == p_nd)
                base_type::m_p_head->m_p_left = p_new_nd;
        } else {

            p_nd->m_p_right = p_new_nd;
            if (base_type::m_p_head->m_p_right == p_nd)
                base_type::m_p_head->m_p_right = p_new_nd;
        }

        p_new_nd->m_p_parent = p_nd;
        p_new_nd->m_p_left = p_new_nd->m_p_right = 0;

        base_type::update_to_top(p_new_nd, (node_update *) this);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __insert(node_pointer p_new_nd, node_pointer p_nd) {
        __base_type__insert_leaf_at(p_new_nd, p_nd);
        p_new_nd->m_red = true;
        insert_fixup(p_new_nd);
        return point_iterator(p_new_nd);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __insert_back(node_pointer p_new_nd) {
        if (base_type::m_size == 0) __base_type__insert_imp_empty(p_new_nd);
        else __base_type__insert_leaf(p_new_nd, base_type::m_p_head->m_p_right, false);
        p_new_nd->m_red = true;
        insert_fixup(p_new_nd);
        return point_iterator(p_new_nd);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __insert_front(node_pointer p_new_nd) {
        if (base_type::m_size == 0) __base_type__insert_imp_empty(p_new_nd);
        else __base_type__insert_leaf(p_new_nd, base_type::m_p_head->m_p_left, true);
        p_new_nd->m_red = true;
        insert_fixup(p_new_nd);
        return point_iterator(p_new_nd);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    insert_fixup(node_pointer p_nd) {
        while (p_nd != base_type::m_p_head->m_p_parent && p_nd->m_p_parent->m_red) {
            if (p_nd->m_p_parent == p_nd->m_p_parent->m_p_parent->m_p_left) {
                node_pointer p_y = p_nd->m_p_parent->m_p_parent->m_p_right;
                if (p_y != 0 && p_y->m_red) {
                    p_nd->m_p_parent->m_red = false;
                        p_y->m_red = false;
                        p_nd->m_p_parent->m_p_parent->m_red = true;
                        p_nd = p_nd->m_p_parent->m_p_parent;
                    } else {
                        if (p_nd == p_nd->m_p_parent->m_p_right) {
                            p_nd = p_nd->m_p_parent;
                            base_type::rotate_left(p_nd);
                        }
                        p_nd->m_p_parent->m_red = false;
                        p_nd->m_p_parent->m_p_parent->m_red = true;
                        base_type::rotate_right(p_nd->m_p_parent->m_p_parent);
                    }
                } else {
                    node_pointer p_y = p_nd->m_p_parent->m_p_parent->m_p_left;
                    if (p_y != 0 && p_y->m_red) {
                        p_nd->m_p_parent->m_red = false;
                        p_y->m_red = false;
                        p_nd->m_p_parent->m_p_parent->m_red = true;
                        p_nd = p_nd->m_p_parent->m_p_parent;
                    } else {
                        if (p_nd == p_nd->m_p_parent->m_p_left) {
                            p_nd = p_nd->m_p_parent;
                            base_type::rotate_right(p_nd);
                        }
                        p_nd->m_p_parent->m_red = false;
                        p_nd->m_p_parent->m_p_parent->m_red = true;
                        base_type::rotate_left(p_nd->m_p_parent->m_p_parent);
                    }
            }
        }

        base_type::update_to_top(p_nd, (node_update *) this);
        base_type::m_p_head->m_p_parent->m_red = false;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_destroy(node_pointer p_nd) {
        erase_node(p_nd);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_destroy_front() {
        if (base_type::m_p_head->m_p_left != base_type::m_p_head)erase_node(base_type::m_p_head->m_p_left);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline void
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_destroy_back() {
        if (base_type::m_p_head->m_p_right != base_type::m_p_head)erase_node(base_type::m_p_head->m_p_right);
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_get_ptr(node_pointer p_nd) {
        remove_node(p_nd);
        --base_type::m_size;
        return p_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_get_ptr_front() {
        node_pointer p_nd = base_type::m_p_head->m_p_left;
        if (p_nd == base_type::m_p_head)return nullptr;
        remove_node(p_nd);
        --base_type::m_size;
        return p_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_get_ptr_back() {
        node_pointer p_nd = base_type::m_p_head->m_p_right;
        if (p_nd == base_type::m_p_head)return nullptr;
        remove_node(p_nd);
        --base_type::m_size;
        return p_nd;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_destroy_get_next(point_iterator it) {
        if (it.m_p_nd == base_type::m_p_head)
            return it;
        point_iterator ret_it = it;
        ++ret_it;
        erase_node(it.m_p_nd);
        return ret_it;
    }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    __erase_destroy_get_prev(point_iterator it) {
        if (it.m_p_nd == base_type::m_p_head)
            return it;
        point_iterator ret_it = it;
        --ret_it;
        erase_node(it.m_p_nd);
        return ret_it;
    }


        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        template<typename Pred>
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::size_type
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        erase_if(Pred pred) {

            size_type num_ersd = 0;
            iterator it = base_type::begin();
            while (it != base_type::end()) {
                if (pred(*it)) {
                    ++num_ersd;
                    it = erase(it);
                } else
                    ++it;
            }

            return num_ersd;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        erase_node(node_pointer p_nd) {
            remove_node(p_nd);
            base_type::actual_erase_node(p_nd);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        remove_node(node_pointer p_z) {
            this->update_min_max_for_erased_node(p_z);
            node_pointer p_y = p_z;
            node_pointer p_x = 0;
            node_pointer p_new_x_parent = 0;

            if (p_y->m_p_left == 0)
                p_x = p_y->m_p_right;
            else if (p_y->m_p_right == 0)
                p_x = p_y->m_p_left;
            else {
                p_y = p_y->m_p_right;
                while (p_y->m_p_left != 0)
                    p_y = p_y->m_p_left;
                p_x = p_y->m_p_right;
            }

            if (p_y == p_z) {
                p_new_x_parent = p_y->m_p_parent;
                if (p_x != 0)
                    p_x->m_p_parent = p_y->m_p_parent;

                if (base_type::m_p_head->m_p_parent == p_z)
                    base_type::m_p_head->m_p_parent = p_x;
                else if (p_z->m_p_parent->m_p_left == p_z) {
                    p_y->m_p_left = p_z->m_p_parent;
                    p_z->m_p_parent->m_p_left = p_x;
                } else {
                    p_y->m_p_left = 0;
                    p_z->m_p_parent->m_p_right = p_x;
                }
            } else {
                p_z->m_p_left->m_p_parent = p_y;
                p_y->m_p_left = p_z->m_p_left;
                if (p_y != p_z->m_p_right) {
                    p_new_x_parent = p_y->m_p_parent;
                    if (p_x != 0)
                        p_x->m_p_parent = p_y->m_p_parent;
                    p_y->m_p_parent->m_p_left = p_x;
                    p_y->m_p_right = p_z->m_p_right;
                    p_z->m_p_right->m_p_parent = p_y;
                } else
                    p_new_x_parent = p_y;

                if (base_type::m_p_head->m_p_parent == p_z)
                    base_type::m_p_head->m_p_parent = p_y;
                else if (p_z->m_p_parent->m_p_left == p_z)
                    p_z->m_p_parent->m_p_left = p_y;
                else
                    p_z->m_p_parent->m_p_right = p_y;

                p_y->m_p_parent = p_z->m_p_parent;
                std::swap(p_y->m_red, p_z->m_red);
                p_y = p_z;
            }

            this->update_to_top(p_new_x_parent, (node_update *) this);

            if (p_y->m_red)
                return;

            remove_fixup(p_x, p_new_x_parent);
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        remove_fixup(node_pointer p_x, node_pointer p_new_x_parent) {

            while (p_x != base_type::m_p_head->m_p_parent && is_effectively_black(p_x))
                if (p_x == p_new_x_parent->m_p_left) {
                    node_pointer p_w = p_new_x_parent->m_p_right;
                    if (p_w->m_red) {
                        p_w->m_red = false;
                        p_new_x_parent->m_red = true;
                        base_type::rotate_left(p_new_x_parent);
                        p_w = p_new_x_parent->m_p_right;
                    }

                    if (is_effectively_black(p_w->m_p_left)
                        && is_effectively_black(p_w->m_p_right)) {
                        p_w->m_red = true;
                        p_x = p_new_x_parent;
                        p_new_x_parent = p_new_x_parent->m_p_parent;
                    } else {
                        if (is_effectively_black(p_w->m_p_right)) {
                            if (p_w->m_p_left != 0)
                                p_w->m_p_left->m_red = false;

                            p_w->m_red = true;
                            base_type::rotate_right(p_w);
                            p_w = p_new_x_parent->m_p_right;
                        }

                        p_w->m_red = p_new_x_parent->m_red;
                        p_new_x_parent->m_red = false;

                        if (p_w->m_p_right != 0)
                            p_w->m_p_right->m_red = false;

                        base_type::rotate_left(p_new_x_parent);
                        this->update_to_top(p_new_x_parent, (node_update *) this);
                        break;
                    }
                } else {
                    node_pointer p_w = p_new_x_parent->m_p_left;
                    if (p_w->m_red == true) {
                        p_w->m_red = false;
                        p_new_x_parent->m_red = true;
                        base_type::rotate_right(p_new_x_parent);
                        p_w = p_new_x_parent->m_p_left;
                    }

                    if (is_effectively_black(p_w->m_p_right)
                        && is_effectively_black(p_w->m_p_left)) {
                        p_w->m_red = true;
                        p_x = p_new_x_parent;
                        p_new_x_parent = p_new_x_parent->m_p_parent;
                    } else {
                        if (is_effectively_black(p_w->m_p_left)) {
                            if (p_w->m_p_right != 0)
                                p_w->m_p_right->m_red = false;

                            p_w->m_red = true;
                            base_type::rotate_left(p_w);
                            p_w = p_new_x_parent->m_p_left;
                        }

                        p_w->m_red = p_new_x_parent->m_red;
                        p_new_x_parent->m_red = false;

                        if (p_w->m_p_left != 0)
                            p_w->m_p_left->m_red = false;

                        base_type::rotate_right(p_new_x_parent);
                        this->update_to_top(p_new_x_parent, (node_update *) this);
                        break;
                    }
                }

            if (p_x != 0)
                p_x->m_red = false;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        join(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &other) {

            if (other.m_size == 0)return;

            if (base_type::m_size == 0) {
                base_type::value_swap(other);
                return;
            }

            const node_pointer p_x = other.split_min();
            join_imp(p_x, other.m_p_head->m_p_parent);
            base_type::join_finish(other);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        join_imp(node_pointer p_x, node_pointer p_r) {
            if (p_r != 0)
                p_r->m_red = false;

            const size_type h = black_height(base_type::m_p_head->m_p_parent);
            const size_type other_h = black_height(p_r);
            node_pointer p_x_l;
            node_pointer p_x_r;
            std::pair<node_pointer, node_pointer> join_pos;
            const bool right_join = h >= other_h;
            if (right_join) {
                join_pos = find_join_pos_right(base_type::m_p_head->m_p_parent,
                                               h, other_h);
                p_x_l = join_pos.first;
                p_x_r = p_r;
            } else {
                p_x_l = base_type::m_p_head->m_p_parent;
                base_type::m_p_head->m_p_parent = p_r;
                if (p_r != 0)
                    p_r->m_p_parent = base_type::m_p_head;

                join_pos = find_join_pos_left(base_type::m_p_head->m_p_parent,
                                              h, other_h);
                p_x_r = join_pos.first;
            }

            node_pointer p_parent = join_pos.second;
            if (p_parent == base_type::m_p_head) {
                base_type::m_p_head->m_p_parent = p_x;
                p_x->m_p_parent = base_type::m_p_head;
            } else {
                p_x->m_p_parent = p_parent;
                if (right_join)
                    p_x->m_p_parent->m_p_right = p_x;
                else
                    p_x->m_p_parent->m_p_left = p_x;
            }

            p_x->m_p_left = p_x_l;
            if (p_x_l != 0)
                p_x_l->m_p_parent = p_x;

            p_x->m_p_right = p_x_r;
            if (p_x_r != 0)
                p_x_r->m_p_parent = p_x;

            p_x->m_red = true;

            base_type::initialize_min_max();

            base_type::update_to_top(p_x, (node_update *) this);
            insert_fixup(p_x);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        split_min() {
            node_pointer p_min = base_type::m_p_head->m_p_left;

            remove_node(p_min);
            return p_min;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        std::pair<
                typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer,
                typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        find_join_pos_right(node_pointer p_l, size_type h_l, size_type h_r) {

            if (base_type::m_p_head->m_p_parent == 0)
                return (std::make_pair((node_pointer) 0, base_type::m_p_head));

            node_pointer p_l_parent = base_type::m_p_head;
            while (h_l > h_r) {
                if (p_l->m_red == false) {
                    --h_l;
                }

                p_l_parent = p_l;
                p_l = p_l->m_p_right;
            }

            if (!is_effectively_black(p_l)) {
                p_l_parent = p_l;
                p_l = p_l->m_p_right;
            }

            return std::make_pair(p_l, p_l_parent);
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        std::pair<
                typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer,
                typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::node_pointer>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        find_join_pos_left(node_pointer p_r, size_type h_l, size_type h_r) {
            if (base_type::m_p_head->m_p_parent == 0)
                return (std::make_pair((node_pointer) 0,
                                       base_type::m_p_head));
            node_pointer p_r_parent = base_type::m_p_head;
            while (h_r > h_l) {
                if (p_r->m_red == false) {
                    --h_r;
                }

                p_r_parent = p_r;
                p_r = p_r->m_p_left;
            }

            if (!is_effectively_black(p_r)) {
                p_r_parent = p_r;
                p_r = p_r->m_p_left;
            }

            return std::make_pair(p_r, p_r_parent);
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::size_type
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        black_height(node_pointer p_nd) {
            size_type h = 1;
            while (p_nd != 0) {
                if (p_nd->m_red == false)
                    ++h;
                p_nd = p_nd->m_p_left;
            }
            return h;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        split(key_const_reference r_key, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &other) {

            if (base_type::split_prep(r_key, other) == false) {

                return;
            }

            node_pointer p_nd = this->upper_bound(r_key).m_p_nd;
            do {
                node_pointer p_next_nd = p_nd->m_p_parent;
                if (Cmp_Fn::operator()(r_key, (p_nd->m_value)))
                    split_at_node(p_nd, other);

                p_nd = p_next_nd;
            } while (p_nd != base_type::m_p_head);

            base_type::split_finish(other);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        split_at_node(node_pointer p_nd, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &other) {

            node_pointer p_l = p_nd->m_p_left;
            node_pointer p_r = p_nd->m_p_right;
            node_pointer p_parent = p_nd->m_p_parent;
            if (p_parent == base_type::m_p_head) {
                base_type::m_p_head->m_p_parent = p_l;
                if (p_l != 0) {
                    p_l->m_p_parent = base_type::m_p_head;
                    p_l->m_red = false;
                }
            } else {
                if (p_parent->m_p_left == p_nd)
                    p_parent->m_p_left = p_l;
                else
                    p_parent->m_p_right = p_l;

                if (p_l != 0)
                    p_l->m_p_parent = p_parent;

                this->update_to_top(p_parent, (node_update *) this);

                if (!p_nd->m_red)
                    remove_fixup(p_l, p_parent);
            }

            base_type::initialize_min_max();
            other.join_imp(p_nd, p_r);

        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        void
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        modify(point_iterator it, const_reference r_value) {
            node_pointer p_nd = it.m_p_nd;
            p_nd->m_value = r_value;
            typedef typename base_type::node_update node_update;
            base_type::update_to_top(p_nd, (node_update *) this);

        }

    template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
    inline bool
    rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
    is_effectively_black(node_pointer p_nd) { return (p_nd == 0 || !p_nd->m_red); }

    }

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


    template<typename Key, template<typename Node_CItr, typename Node_Itr,
            typename Cmp_Fn_, typename _Alloc_>
            class Node_Update = __gnu_pbds::null_node_update, typename _Alloc = std::allocator<char> >
    using tree_list = __gnu_pbds::detail::rb_tree_list<Key,
            __gnu_pbds::null_type, detail::null_comparer<Key>, __gnu_pbds::detail::tree_traits<Key, __gnu_pbds::null_type, detail::null_comparer<Key>, Node_Update, __gnu_pbds::rb_tree_tag, _Alloc>, _Alloc>;
    //__gnu_pbds::detail::rb_tree_set<int, __gnu_pbds::null_type, std::less<int>, __gnu_pbds::detail::tree_traits<int, __gnu_pbds::null_type, std::less<int>, __gnu_pbds::tree_order_statistics_node_update, __gnu_pbds::rb_tree_tag, std::allocator<char> >, std::allocator<char> >
}

#include <iostream>

namespace alg {
    namespace detail {
        template<typename T>
        struct max_prefix_metadata {
            T sum, max_prefix;
        };
    }

        template<typename Node_CItr, typename Node_Itr, typename Cmp_Fn, typename _Alloc>
    class max_prefix_policy : private __gnu_pbds::detail::branch_policy<Node_CItr, Node_Itr, _Alloc> {
    private:
        typedef __gnu_pbds::detail::branch_policy<Node_CItr, Node_Itr, _Alloc> base_type;
        public:

            typedef typename base_type::key_type key_type;
            typedef typename base_type::key_const_reference key_const_reference;

            typedef Node_CItr node_const_iterator;
            typedef Node_Itr node_iterator;
            typedef typename node_iterator::value_type iterator;
            typedef typename std::remove_pointer_t<iterator> value_type;
            typedef detail::max_prefix_metadata<key_type> metadata_type;
            typedef metadata_type &metadata_reference;

            void operator()(Node_Itr nd_it, Node_CItr end_nd_it) {
                node_iterator l_it = nd_it.get_l_child();
                node_iterator r_it = nd_it.get_r_child();
                const metadata_type l_m = (l_it == end_nd_it) ? metadata_type{0, 0} : l_it.get_metadata();
                const metadata_type r_m = (r_it == end_nd_it) ? metadata_type{0, 0} : r_it.get_metadata();
                const_cast<metadata_reference >(nd_it.get_metadata()) = metadata_type{.sum = l_m.sum + **nd_it +
                                                                                             r_m.sum, .max_prefix = std::max(
                        l_m.max_prefix, l_m.sum + **nd_it + r_m.max_prefix)};
            }

            key_type get_max_prefix() {
                Node_CItr it = node_begin();
                return it.get_metadata().max_prefix;
            }

            virtual Node_CItr
            node_begin() const = 0;

            virtual Node_CItr
            node_end() const = 0;
        };

}

namespace alg {
    template<typename Node_CItr, typename Node_Itr,
            typename Cmp_Fn, typename _Alloc>
    class random_access_policy
            : public __gnu_pbds::tree_order_statistics_node_update<Node_CItr, Node_Itr, Cmp_Fn, _Alloc> {

    private:
        typedef __gnu_pbds::tree_order_statistics_node_update<Node_CItr, Node_Itr, Cmp_Fn, _Alloc> base_type;
    public:

        typename base_type::key_const_reference operator[](typename base_type::size_type idx) const {
            return *base_type::find_by_order(idx);
        }

    };

}

#endif //TREES_TREE_LIST_H

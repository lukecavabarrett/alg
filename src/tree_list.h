#ifndef TREES_TREE_LIST_H
#define TREES_TREE_LIST_H

namespace alg {
    namespace detail {
        template<class T = void>
        struct null_comparer {
            typedef bool result_type;
            typedef T first_argument_type;
            typedef T second_argument_type;
            // constexpr bool operator()(const T &lhs, const T &rhs) const {}
        };
    }
}

#include <ext/pb_ds/assoc_container.hpp>


namespace __gnu_pbds {
    namespace detail {
        template<typename Key,
                typename Mapped,
                typename Cmp_Fn,
                typename Node_And_It_Traits,
                typename _Alloc>
    class rb_tree_list : public bin_search_tree_set<Key, __gnu_pbds::null_type, alg::detail::null_comparer<Key>, Node_And_It_Traits, _Alloc> {
        private:
            typedef bin_search_tree_set<Key, __gnu_pbds::null_type, alg::detail::null_comparer<Key>, Node_And_It_Traits, _Alloc> base_type;
            typedef typename base_type::node_pointer node_pointer;

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

            typedef node_pointer public_node_pointer;

            rb_tree_list();

            rb_tree_list(const Cmp_Fn &);

            rb_tree_list(const Cmp_Fn &, const node_update &);

            rb_tree_list(const rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

            void
            swap(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

            template<typename It>
            void
            copy_from_range(It, It);

            //inline std::pair<point_iterator, bool>            insert(const_reference);

            inline point_iterator
            __base_type__insert_leaf_at(const_reference, node_pointer);

            point_iterator
            insert_at(point_iterator, const_reference);

            inline iterator
            erase(iterator);

            inline reverse_iterator
            erase(reverse_iterator);

            template<typename Pred>
            inline size_type
            erase_if(Pred);

            void
            join(rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

            void
            split(key_const_reference, rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc> &);

        private:
            inline static bool
            is_effectively_black(const node_pointer);

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
        typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        insert_at(point_iterator it, const_reference r_value) {

            point_iterator itn = __base_type__insert_leaf_at(r_value, it.m_p_nd);
            itn.m_p_nd->m_red = true;
            insert_fixup(itn.m_p_nd);
            return itn;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        __base_type__insert_leaf_at(const_reference r_value, node_pointer x) {
            if (base_type::m_size == 0) return base_type::insert_imp_empty(r_value);
            if (x == base_type::m_p_head) return base_type::insert_leaf_new(r_value, base_type::m_p_head->m_p_right, false);
            if (x->m_p_left == nullptr)return base_type::insert_leaf_new(r_value, x, true);
            x = x->m_p_left;
            while(x->m_p_right != nullptr)x = x->m_p_right;
            return base_type::insert_leaf_new(r_value, x, false);
        }
        /*
        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline std::pair<typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::point_iterator, bool>
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        insert(const_reference r_value) {

            std::pair<point_iterator, bool> ins_pair = base_type::insert_leaf(r_value);
            if (ins_pair.second == true) {
                ins_pair.first.m_p_nd->m_red = true;

                insert_fixup(ins_pair.first.m_p_nd);
            }

            return ins_pair;
        }*/

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
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::iterator
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        erase(iterator it) {

            if (it == base_type::end())
                return it;

            iterator ret_it = it;
            ++ret_it;
            erase_node(it.m_p_nd);

            return ret_it;
        }

        template<typename Key, typename Mapped, typename Cmp_Fn, typename Node_And_It_Traits, typename _Alloc>
        inline typename rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::reverse_iterator
        rb_tree_list<Key, Mapped, Cmp_Fn, Node_And_It_Traits, _Alloc>::
        erase(reverse_iterator it) {

            if (it.m_p_nd == base_type::m_p_head)
                return it;

            reverse_iterator ret_it = it;
            ++ret_it;
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
        is_effectively_black(const node_pointer p_nd) { return (p_nd == 0 || !p_nd->m_red); }

    }
}


namespace alg {


    template<typename Key, template<typename Node_CItr, typename Node_Itr,
            typename Cmp_Fn_, typename _Alloc_>
            class Node_Update = __gnu_pbds::null_node_update, typename _Alloc = std::allocator<char> >
    using tree_list = __gnu_pbds::detail::rb_tree_list<Key,
    __gnu_pbds::null_type, detail::null_comparer<Key>, __gnu_pbds::detail::tree_traits<Key, __gnu_pbds::null_type, detail::null_comparer<Key>, Node_Update, __gnu_pbds::rb_tree_tag, _Alloc >, _Alloc>;
    //__gnu_pbds::detail::rb_tree_set<int, __gnu_pbds::null_type, std::less<int>, __gnu_pbds::detail::tree_traits<int, __gnu_pbds::null_type, std::less<int>, __gnu_pbds::tree_order_statistics_node_update, __gnu_pbds::rb_tree_tag, std::allocator<char> >, std::allocator<char> >
}

#endif //TREES_TREE_LIST_H

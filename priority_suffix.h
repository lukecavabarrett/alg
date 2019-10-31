#ifndef ALG_MAX_SUFFIX_H
#define ALG_MAX_SUFFIX_H

#include <map>

namespace alg {

template<
        class Key,
        class T,
        class CompareKey = std::less<Key>,
        class Compare = std::less<T>,
        class Allocator = std::allocator<std::pair<const Key,const T> >
>
class priority_suffix : private std::multimap<Key,const T, CompareKey, Allocator> {

    const Compare c;
    typedef std::multimap<Key,const T, CompareKey, Allocator> parent;

public:

    typedef std::pair<const Key,const T> value_type;
    using  parent::allocator_type;
    using  parent::size_type;
    using  parent::difference_type;
    using  parent::reference;
    using  parent::const_reference;
    using  parent::pointer;
    using  parent::const_pointer;
    using  parent::iterator;
    using  parent::const_iterator;

    //ctors
    priority_suffix() : parent(), c() {}
    explicit priority_suffix(const CompareKey& comp) : parent(comp), c() {}

    void insert(const Key &k, const T &v);
    using parent::upper_bound;
    using parent::lower_bound;
    using parent::begin;
    using parent::cbegin;
    using parent::end;
    using parent::cend;
    using parent::rbegin;
    using parent::crbegin;
    using parent::rend;
    using parent::crend;
    using parent::size;
    using parent::get_allocator;
    using parent::empty;
    using parent::max_size;
    using parent::clear;
    using parent::swap;
};

template<class Key, class T, class CompareKey, class Compare, class Allocator>
void priority_suffix<Key, T, CompareKey, Compare, Allocator>::insert(const Key &k, const T &v) {
    auto p = parent::insert(std::make_pair(k, v));
    auto p_next = p;
    ++p_next;

    if (p_next != parent::end() && c(v, p_next->second)) {
        parent::erase(p);
        return;
    }

    while (p != parent::begin()) {
        auto p_prev = p;
        --p_prev;
        if (c(p_prev->second, v))
            parent::erase(p_prev);
        else
            return;
    }
}
}

#endif //ALG_MAX_SUFFIX_H

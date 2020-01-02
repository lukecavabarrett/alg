#ifndef ALG_ROLLING_HASH_MULTI_H
#define ALG_ROLLING_HASH_MULTI_H

#include <cmath>
#include <cstdint>
#include <typeindex>
#include "rolling_hash.h"

namespace alg {

template <size_t I,typename T>
struct tuple_n{
    template< typename...Args> using type = typename tuple_n<I-1, T>::template type<T, Args...>;
};

template <typename T>
struct tuple_n<0, T> {
    template<typename...Args> using type = std::tuple<Args...>;
};
template <size_t I,typename T>  using tuple_of = typename tuple_n<I,T>::template type<>;

template<class T = char, unsigned int K = 3, class S= uint64_t, S B= next_prime((1<<(sizeof(T)*8))) >
class rolling_hash_multi;

template<class T, class S , S B>
class rolling_hash_multi<T,0,S,B> {
    typedef rolling_hash<T,S,B> hash_class;
public:
    using hash_class::hash_class;
    static constexpr unsigned int hardness = 0;
    typedef tuple_of<0,S> hash_type;
    static constexpr hash_type base_value = hash_type();
};

template<class T, class S , S B>
class rolling_hash_multi<T,1,S,B> : public rolling_hash<T,S,B> {
    typedef rolling_hash<T,S,B> hash_class;
public:
    using hash_class::hash_class;
    static constexpr unsigned int hardness = 1;
    typedef tuple_of<1,S> hash_type;
    static constexpr hash_type base_value = hash_type(B);
    hash_type raw() const {return hash_type(hash_class::raw());}
};

template<class T, unsigned int K, class S , S B>
class rolling_hash_multi : public rolling_hash<T,S,B>, public rolling_hash_multi<T,K-1,S,next_prime(B+1)> {
    typedef rolling_hash<T,S,B> hash_class;
    typedef rolling_hash_multi<T,K-1,S,next_prime(B+1)> inner_type;
public:
    static constexpr unsigned int hardness = K;
    typedef tuple_of<K,S> hash_type;
    static constexpr hash_type base_value = std::tuple_cat(std::tuple<S>(B),inner_type::base_value);

    hash_type raw() const {return std::tuple_cat(std::tuple<S>(hash_class::raw()),inner_type::raw());}
    rolling_hash_multi() : hash_class(), inner_type() {}
    rolling_hash_multi(const rolling_hash_multi& o) : hash_class(o), inner_type(o) {}
    explicit rolling_hash_multi(const T& c) : hash_class(c), inner_type(c) {}
    template <class InputIt>
    rolling_hash_multi(InputIt first,InputIt last) : hash_class(first,last), inner_type(first,last){}
    template <class Range>
    explicit rolling_hash_multi(Range s) : hash_class(s), inner_type(s) {};
    bool operator==(const rolling_hash_multi& o) const {return hash_class::operator==(o) && inner_type::operator==(o);}
    bool operator!=(const rolling_hash_multi& o) const {return hash_class::operator!=(o) || inner_type::operator!=(o);}
    rolling_hash_multi& append(const rolling_hash_multi& o) {hash_class::append(o);inner_type::append(o);
        return *this;}
    template <class InputIt>
    rolling_hash_multi& append(InputIt first,InputIt last) {hash_class::append(first,last);inner_type::append(first,last);
        return *this;}
    template <class Range>
    rolling_hash_multi& append(const Range& s) {hash_class::append(s);inner_type::append(s);
        return *this;}
    rolling_hash_multi& prepend(const rolling_hash_multi& o){hash_class::prepend(o);inner_type::prepend(o);
        return *this;}
    template <class InputIt>
    rolling_hash_multi& prepend(InputIt first,InputIt last){hash_class::prepend(first,last);inner_type::prepend(first,last);
        return *this;}
    //inline hash_type raw() const {return hash;}
    rolling_hash_multi& operator+=(const rolling_hash_multi& o){hash_class::operator+=(o);inner_type::operator+=(o);return *this;};
    rolling_hash_multi operator+(const rolling_hash_multi& o){rolling_hash_multi h(*this);h.append(o);return h;}
};

}
#endif //ALG_ROLLING_HASH_MULTI_H

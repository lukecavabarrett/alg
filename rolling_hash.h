#ifndef ALG_ROLLING_HASH_H
#define ALG_ROLLING_HASH_H

#include <cmath>
#include <stdint-gcc.h>

namespace alg {

namespace {
 constexpr bool is_prime(int n){
     if(n<3)return false;
     for(int k=2;k<=std::sqrt(n);++k)if(n%k==0)return false;
     return true;
 }
 constexpr std::size_t next_prime(std::size_t x){
     while(!is_prime(x))++x;
     return x;
 }
}


template<class T = char, class S = uint64_t , S B = next_prime((1<<(sizeof(T)*8)))>
class rolling_hash {
public:
    typedef T value_type;
    typedef S hash_type;
    static constexpr S base_value = B;
    rolling_hash() : hash(0), b_size(1) {}
    rolling_hash(const rolling_hash& o) : hash(o.hash), b_size(o.b_size) {}
    explicit rolling_hash(const T& c) : hash(c), b_size(B) {}
    template <class InputIt>
    rolling_hash(InputIt first,InputIt last);
    template <class Range>
    explicit rolling_hash(const Range& s) : rolling_hash(s.begin(),s.end()) {};
    bool operator==(const rolling_hash& o) const {return hash==o.hash;}
    bool operator!=(const rolling_hash& o) const {return hash!=o.hash;}
    rolling_hash& append(const rolling_hash& o);
    template <class InputIt>
    rolling_hash& append(InputIt first,InputIt last);
    template <class Range>
    rolling_hash& append(const Range& s){return append(s.begin(),s.end());}
    rolling_hash& prepend(const rolling_hash& o);
    template <class InputIt>
    rolling_hash& prepend(InputIt first,InputIt last);
    inline hash_type raw() const {return hash;}
    rolling_hash& operator+=(const rolling_hash& o);
    rolling_hash operator+(const rolling_hash& o);
    struct hasher{
        typedef rolling_hash argument_type;
        typedef std::size_t result_type;
        result_type operator()(const argument_type& a) const {return a.hash;}
    };
private:
    S hash,b_size;
};

template<class T, class S, S B>
template<class InputIt>
rolling_hash<T, S, B>::rolling_hash(InputIt first, InputIt last) : rolling_hash() {
    while(first!=last){
        hash*=B;
        b_size*=B;
        hash+=*first;
        ++first;
    }
}

template<class T, class S, S B>
rolling_hash<T,S,B> &rolling_hash<T, S, B>::operator+=(const rolling_hash &o) {
    hash*=o.b_size;
    b_size*=o.b_size;
    hash+=o.hash;
    return *this;
}

template<class T, class S, S B>
rolling_hash<T,S,B> rolling_hash<T, S, B>::operator+(const rolling_hash &o) {
    rolling_hash h(*this);
    h+=o;
    return  h;
}

template<class T, class S, S B>
template<class InputIt>
rolling_hash<T,S,B> &rolling_hash<T, S, B>::append(InputIt first, InputIt last) {
    while(first!=last){
        hash*=B;
        b_size*=B;
        hash+=*first;
        ++first;
    }
    return *this;
}

template<class T, class S, S B>
rolling_hash<T,S,B> &rolling_hash<T, S, B>::append(const rolling_hash &o) {
    hash*=o.b_size;
    b_size*=o.b_size;
    hash+=o.hash;
    return *this;
}

template<class T, class S, S B>
rolling_hash<T,S,B> &rolling_hash<T, S, B>::prepend(const rolling_hash &o) {
    hash+=o.hash*b_size;
    b_size*=o.b_size;
    return *this;
}

template<class T, class S, S B>
template<class InputIt>
rolling_hash<T,S,B> &rolling_hash<T, S, B>::prepend(InputIt first, InputIt last) {
    return this->prepend(rolling_hash<T,S,B>(first,last));
}

}
#endif //ALG_ROLLING_HASH_H

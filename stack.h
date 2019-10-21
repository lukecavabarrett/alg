#ifndef ALG_STACK_H
#define ALG_STACK_H

#include <cstddef>
#include <memory>
#include "vector.h"

namespace alg {

template<typename T, typename Container = vector<T> >
class stack : private Container {
    typedef Container base_class;
public:
    using Container::Container;
    using Container::operator=;
    using Container::empty;
    using Container::size;
    inline auto top() {return Container::back();}
    inline void push(const T& v) {Container::push_back(v);}
    inline void push(T&& v) {Container::push_back(std::move(v));}
    template< class... Args >
    decltype(auto) emplace( Args&&... args ){return Container::emplace_back(std::forward<Args>(args)...);}
};

}

#endif //ALG_STACK_H

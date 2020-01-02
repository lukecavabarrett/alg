#ifndef ALG_PRIORITY_QUEUE_H
#define ALG_PRIORITY_QUEUE_H

#include "vector.h"

namespace alg{



template<typename T, typename Container = vector<T>, typename Comp = std::less<T> >
class priority_queue : private Container {
    typedef Container base_class;
    inline void i_make_heap(){
        std::make_heap(Container::begin(),Container::end(),Comp());
    };
    inline void i_push_heap(){
        std::push_heap(Container::begin(),Container::end(),Comp());
    };
    inline void i_pop_heap(){
        std::pop_heap(Container::begin(),Container::end(),Comp());
    };
public:
    using Container::operator=;
    using Container::empty;
    using Container::size;
    using Container::clear;
    template< class... Args >
    explicit priority_queue(Args&&... args) : Container(std::forward<Args>(args)...) {i_make_heap();}
    inline auto top() {return Container::front();}
    inline void push(const T& v) {Container::push_back(v); i_push_heap(); }
    inline void push(T&& v) {Container::push_back(std::move(v)); i_push_heap(); }
    template< class... Args >
    decltype(auto) emplace( Args&&... args ){return Container::emplace_back(std::forward<Args>(args)...); i_push_heap();}
    inline void pop() {i_pop_heap(); Container::pop_back();}
    void swap( priority_queue& other ) noexcept {Container::swap(other);}
    bool operator==(const priority_queue& o) const {return static_cast<Container>(*this)==static_cast<Container>(o);}
    bool operator!=(const priority_queue& o) const {return static_cast<Container>(*this)!=static_cast<Container>(o);}
};

}

#endif //ALG_PRIORITY_QUEUE_H

#ifndef ALG_BINARY_NODE_H
#define ALG_BINARY_NODE_H

#include <utility>


namespace alg {
template<typename T>
struct binary_node {
    typedef T value_type;
    value_type value;
    binary_node *left, *right;

    explicit binary_node(const T &v) : value(v), left(nullptr), right(nullptr){}
    explicit binary_node(T &&v) : value(v), left(nullptr), right(nullptr){}
    template<class... Args>
    binary_node(Args &&... args) : value(std::forward<Args>(args)...),left(nullptr), right(nullptr){}
    //~binary_node(){if(left)delete left;if(right)delete right;}

};
}

#endif //ALG_BINARY_NODE_H

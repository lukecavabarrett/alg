#ifndef ALG_TREE_ALGS_H
#define ALG_TREE_ALGS_H

#include <functional>
#include <stack>
#include "alg/binary_node.h"

namespace alg{

template<typename It,class Cmp=std::less< typename std::iterator_traits<It>::value_type::value_type > >
typename std::iterator_traits<It>::value_type *make_cartesian_tree(It first,It last){
    typedef typename std::iterator_traits<It>::value_type bnode;
    typedef typename bnode::value_type value_type;
    static_assert(std::is_same<binary_node<value_type>,bnode >::value);
    typedef bnode *pointer;
    std::stack<pointer> right_dorsal;
    Cmp comp;
    for(;first!=last;++first){
        pointer right_child = nullptr;
        pointer x = &*first;
        while(!right_dorsal.empty() && comp(x->value,right_dorsal.top()->value)){
            right_child = right_dorsal.top();
            right_dorsal.pop();
        }
        if(!right_dorsal.empty())right_dorsal.top()->right=x;
        first->left=right_child;
        first->right = nullptr;
        right_dorsal.push(x);
    }

    //recover root
    pointer root = nullptr;
    while(!right_dorsal.empty()){
        root=right_dorsal.top();
        right_dorsal.pop();
    }

    return root;
}

}


#endif //ALG_TREE_ALGS_H

#ifndef ALG_FIBONACCI_HEAP_H
#define ALG_FIBONACCI_HEAP_H

#include <functional>

namespace alg{

template<class Key,class Compare = std::less<Key>, class Allocator = std::allocator<Key> >
class fibonacci_set{
public:
    typedef Key key_type;
    typedef const key_type value_type;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef std::uint8_t degree_type;
    inline degree_type max_d(degree_type a,degree_type b){return a<b?b:a;}
    typedef bool flag_type;
    struct node;
    typedef node* pointer;
    typedef const node* const_pointer;
    struct node{
        key_type value;
        flag_type loser;
        degree_type degree;
        pointer parent,child,prev,succ;
        node(const key_type& k)
                :value(k),loser(false),degree(0),parent(nullptr),child(nullptr),prev(nullptr),succ(nullptr){}
    };

public:

    class iterator{
        friend class fibonacci_set;
    private:
        pointer _node;
    public:
        iterator():_node(nullptr){}
        iterator(const pointer& p):_node(p){}
        bool uninitialized(){return !_node;}
        const_reference operator*() const
        { return (_node)->value; }
        bool operator==(const iterator& o) const {return _node==o._node;}
    };

private:
    pointer root;
    size_type _size;
    Compare _comp;

    void i_debug(pointer x,int d=0)
    {
        for(int w=d;w--;)fputs("   ",stderr);
        fprintf(stderr,"(%d):%d l%d d%d\n",x->value,x->loser,x->degree);
        if(x->child){
            i_debug(x->child,d+1);
            for(pointer y=x->child->succ;y!=x->child;y=y->succ)i_debug(y,d+1);
        }
    }

public:

    fibonacci_set() : root(nullptr), _size(0),_comp() {}
    bool empty(){return _size==0;}
    size_type size(){return _size;}
    iterator push(const key_type& key);
    iterator top()
    {
        assert(_size);
        return iterator(root);
    }
    void pop()
    {
        assert(_size);

        //1. put all children of root after it
        if(root->child)
        {
            pointer x = root->child;
            x->parent = nullptr;
            x->loser = false;
            for(pointer y=x->succ;y!=x;y=y->succ)
            {
                y->parent=nullptr;
                y->loser=false;
            }
            pointer xe = x->prev;
            x->prev = root;
            xe->succ=root->succ;
            x->prev->succ=x;
            xe->succ->prev=xe;
        }
        //2. empty heap
        if(root->prev==root)
        {
            --_size;
#ifdef SAFE_ASSERTIONS
            assert(!_size);
            assert(!root->degree);
            assert(!root->child);
#endif
            //delete[] root;
            root=nullptr;
            return;
        }
        degree_type max_degree=0;
        size_type n_roots=0;
        for(pointer x=root->succ;x!=root;x=x->succ,++n_roots)max_degree=max_d(max_degree,x->degree);
        ++max_degree;
        while(n_roots>>=1)++max_degree;
        pointer pool[max_degree];
        //fprintf(stderr,"maxdegree: %d\n",max_degree);
        memset(pool,0,max_degree*sizeof(pointer));
        for(pointer it=root->succ;it!=root;)
        {
            pointer x=it;it=it->succ;
            degree_type x_deg=x->degree;
            //x is a node to be placed
#ifdef SAFE_ASSERTIONS
            assert(x_deg<max_degree);
#endif
            //fprintf(stderr,"placing %d (d%d)\n",x->value.first,x_deg);
            while(pool[x_deg])
            {
                pointer y=pool[x_deg];
                //fprintf(stderr,"occupied by %d (d%d)\n",y->value.first,y->degree);
                pool[x_deg]=nullptr;
                if(_comp(y->value,x->value))std::swap(x,y);
                ++x_deg;x->degree++;
                y->parent=x;
                if(x->child==nullptr)
                {
                    y->prev=y->succ=y;
                    x->child=y;
                }
                else
                {
                    y->prev=x->child;
                    y->succ=x->child->succ;
                    x->child->succ=y;
                    y->succ->prev=y;
                }
#ifdef SAFE_ASSERTIONS
                assert(x_deg<max_degree);
#endif
                //fprintf(stderr,"placing %d (d%d)\n",x->value.first,x_deg);
            }
            pool[x_deg]=x;
        }
        --_size;
        //delete[] root;
        root=nullptr;
        for(degree_type i=0;i<max_degree;i++)if(pool[i])
            {
                pointer x=pool[i];
                if(!root)
                {
                    root=x;
                    x->succ=x->prev=x;
                }
                else
                {
                    x->prev = root;
                    x->succ = root->succ;
                    x->succ->prev=x;
                    root->succ=x;
                    if(_comp(x->value,root->value))root=x;
                }
            }
    }
    void decrease_key(const iterator& it,const key_type k)
    {
        pointer x = it._node;
        if(_comp(x->value,k))throw;
        x->value=k;
        if(!x->parent){
            if(_comp(x->value,root->value))root=x;
            return;
        }
        if(!_comp(k,x->parent->value))return;
        //must detach x
        while(x->parent)
        {
            pointer p = x->parent;
            p->degree--;
            if(x->succ==x)//only child
            {
#ifdef SAFE_ASSERTIONS
                assert(!p->degree);
#endif
                p->child=nullptr;
            }
            else
            {
                if(p->child==x)p->child=x->succ;
                x->succ->prev=x->prev;
                x->prev->succ=x->succ;
            }
            x->parent=nullptr;
            x->prev = root;
            x->succ = root->succ;
            x->succ->prev=x;
            root->succ=x;
            if(_comp(x->value,root->value))root=x;
            p->loser^=1;
            if(p->loser)break;
            x=p;
        }
    }
    void debug()
    {
        i_debug(root,0);
        for(pointer x=root->succ;x!=root;x=x->succ)i_debug(x,0);
    }
};

template<class Key, class Compare, class Allocator>
typename fibonacci_set<Key, Compare,Allocator>::iterator fibonacci_set<Key, Compare, Allocator>::push(const key_type &key) {
    ++_size;
    if(root==nullptr)
    {
        root = new node(key);
        root->succ=root->prev=root;
        return iterator(root);
    }
    pointer x = new node(key);
    //insert x after root
    x->prev = root;
    x->succ = root->succ;
    x->succ->prev=x;
    root->succ=x;
    if(_comp(x->value,root->value))root=x;
    return iterator(x);
}

template<class Key,class T,class Compare = std::less<Key>,class Allocator = std::allocator<std::pair<Key,T>> >
class fibonacci_map{
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<const Key, T> value_type;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef std::uint8_t degree_type;
    inline degree_type max_d(degree_type a,degree_type b){return a<b?b:a;}
    typedef bool flag_type;
    struct node;
    typedef node* pointer;
    typedef const node* const_pointer;
    struct node{
        std::pair<Key, T> value;
        flag_type loser;
        degree_type degree;
        pointer parent,child,prev,succ;
        node(const key_type& k,const mapped_type& m)
                :value(k,m),loser(false),degree(0),parent(nullptr),child(nullptr),prev(nullptr),succ(nullptr){}
    };

public:

    class iterator{
        friend class fibonacci_map;
    private:
        pointer _node;
    public:
        iterator():_node(nullptr){}
        iterator(const pointer& p):_node(p){}
        bool uninitialized(){return !_node;}
        value_type* operator->() const
        { return (value_type*)std::addressof(_node->value);}
        reference operator*() const
        { return *(value_type*)std::addressof(_node->value);}
    };

private:
    pointer root;
    size_type _size;
    Compare _comp;

    void i_debug(pointer x,int d=0)
    {
        for(int w=d;w--;)fputs("   ",stderr);
        fprintf(stderr,"(%d):%d l%d d%d\n",x->value.first,x->value.second,x->loser,x->degree);
        if(x->child)
        {
            i_debug(x->child,d+1);
            for(pointer y=x->child->succ;y!=x->child;y=y->succ)i_debug(y,d+1);
        }
    }

public:

    fibonacci_map(){root=nullptr;_size=0;}
    bool empty(){return !_size;}
    size_type size(){return _size;}
    iterator push(const key_type& key, const mapped_type& map)
    {
        ++_size;
        if(root==nullptr)
        {
            root = new node(key,map);
            root->succ=root->prev=root;
            return iterator(root);
        }
        pointer x = new node(key,map);
        //insert x after root
        x->prev = root;
        x->succ = root->succ;
        x->succ->prev=x;
        root->succ=x;
        if(_comp(x->value.first,root->value.first))root=x;
        return iterator(x);
    }
    iterator top()
    {
        assert(_size);
        return iterator(root);
    }
    void pop()
    {
        assert(_size);

        //1. put all children of root after it
        if(root->child)
        {
            pointer x = root->child;
            x->parent = nullptr;
            x->loser = false;
            for(pointer y=x->succ;y!=x;y=y->succ)
            {
                y->parent=nullptr;
                y->loser=false;
            }
            pointer xe = x->prev;
            x->prev = root;
            xe->succ=root->succ;
            x->prev->succ=x;
            xe->succ->prev=xe;
        }
        //2. empty heap
        if(root->prev==root)
        {
            --_size;
#ifdef SAFE_ASSERTIONS
            assert(!_size);
            assert(!root->degree);
            assert(!root->child);
#endif
            //delete[] root;
            root=nullptr;
            return;
        }
        degree_type max_degree=0;
        size_type n_roots=0;
        for(pointer x=root->succ;x!=root;x=x->succ,++n_roots)max_degree=max_d(max_degree,x->degree);
        ++max_degree;
        while(n_roots>>=1)++max_degree;
        pointer pool[max_degree];
        //fprintf(stderr,"maxdegree: %d\n",max_degree);
        memset(pool,0,max_degree*sizeof(pointer));
        for(pointer it=root->succ;it!=root;)
        {
            pointer x=it;it=it->succ;
            degree_type x_deg=x->degree;
            //x is a node to be placed
#ifdef SAFE_ASSERTIONS
            assert(x_deg<max_degree);
#endif
            //fprintf(stderr,"placing %d (d%d)\n",x->value.first,x_deg);
            while(pool[x_deg])
            {
                pointer y=pool[x_deg];
                //fprintf(stderr,"occupied by %d (d%d)\n",y->value.first,y->degree);
                pool[x_deg]=nullptr;
                if(_comp(y->value.first,x->value.first))std::swap(x,y);
                ++x_deg;x->degree++;
                y->parent=x;
                if(x->child==nullptr)
                {
                    y->prev=y->succ=y;
                    x->child=y;
                }
                else
                {
                    y->prev=x->child;
                    y->succ=x->child->succ;
                    x->child->succ=y;
                    y->succ->prev=y;
                }
#ifdef SAFE_ASSERTIONS
                assert(x_deg<max_degree);
#endif
                //fprintf(stderr,"placing %d (d%d)\n",x->value.first,x_deg);
            }
            pool[x_deg]=x;
        }
        --_size;
        //delete[] root;
        root=nullptr;
        for(degree_type i=0;i<max_degree;i++)if(pool[i])
            {
                pointer x=pool[i];
                if(!root)
                {
                    root=x;
                    x->succ=x->prev=x;
                }
                else
                {
                    x->prev = root;
                    x->succ = root->succ;
                    x->succ->prev=x;
                    root->succ=x;
                    if(_comp(x->value.first,root->value.first))root=x;
                }
            }
    }
    void decrease_key(const iterator& it,const key_type k)
    {
        pointer x = it._node;
        if(_comp(x->value.first,k))throw;
        x->value.first=k;
        if(!x->parent){
            if(_comp(x->value.first,root->value.first))root=x;
            return;
        }
        if(!_comp(k,x->parent->value.first))return;
        //must detach x
        while(x->parent)
        {
            pointer p = x->parent;
            p->degree--;
            if(x->succ==x)//only child
            {
                assert(!p->degree);
                p->child=nullptr;
            }
            else
            {
                if(p->child==x)p->child=x->succ;
                x->succ->prev=x->prev;
                x->prev->succ=x->succ;
            }
            x->parent=nullptr;
            x->prev=root;
            x->succ=root->succ;
            x->succ->prev=x;
            root->succ=x;
            if(_comp(x->value.first,root->value.first))root=x;
            p->loser^=1;
            if(p->loser)break;
            x=p;
        }
    }
    void debug(){
        i_debug(root,0);
        for(pointer x=root->succ;x!=root;x=x->succ)i_debug(x,0);
    }
};

}

#endif //ALG_FIBONACCI_HEAP_H

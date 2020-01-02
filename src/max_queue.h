#ifndef ALG_MAX_QUEUE_H
#define ALG_MAX_QUEUE_H

#include <deque>
#include <functional>

namespace alg {

template<class T, class Compare = std::less<T>, class S = std::size_t, class Container = std::deque< std::pair<const S, const T> > >
class max_queue {
public:

    max_queue() : _c(), _comp(), _begin(0), _end(0) {}

    template<class InputIt>
    max_queue(InputIt first,InputIt last);

    void push(const T &);

    void push(T &&);

    const T &top() { return _c.front().second; }

    bool empty() { return _begin == _end; }

    S size() { return _end - _begin; }

    S begin() { return _begin;}
    S end() { return _end;}

    void pop();

private:
    Container _c;
    Compare _comp;
    S _begin, _end;
};

template<class T, class Compare,class S, class Container>
void max_queue<T, Compare,S, Container>::push(const T &x) {
    while (!_c.empty() && _comp(_c.back().second, x))_c.pop_back();
    _c.emplace_back(_end++, x);
}

template<class T, class Compare, class S, class Container>
void max_queue<T,Compare, S, Container>::push(T &&x) {
    while (!_c.empty() && _comp(_c.back().second, x))_c.pop_back();
    _c.emplace_back(_end++, std::move(x));
}

template<class T, class Compare, class S, class Container>
void max_queue<T,Compare, S, Container>::pop() {
    if (_c.front().first == _begin)_c.pop_front();
    ++_begin;
}

template<class T, class Compare, class S, class Container>
template<class InputIt>
max_queue<T,Compare, S, Container>::max_queue(InputIt first, InputIt last) : max_queue() {
    for(;first!=last;++first)push(*first);
}
}
#endif //ALG_MAX_QUEUE_H

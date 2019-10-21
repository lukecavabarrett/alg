#ifndef ALG_VECTOR_H
#define ALG_VECTOR_H

#include <cstddef>

namespace alg {

template<typename T, typename Allocator = std::allocator<T> >
class vector {
public:
    typedef T value_type;
    typedef typename Allocator::template rebind<T>::other allocator_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
private:
    static allocator_type _allocator;
    size_t _size, _capacity;
    T *_data;

    void reserve_internal(size_t nsize);

    void reserve_for_insertion() { if (_size == _capacity)reserve_internal((_size << 1) | 1); }

    void reserve_for_insertion(size_t size) {
        if (_size + size > _capacity)
            reserve_internal(std::max(_size << 1, _size + size));
    }

public:
    vector();

    ~vector();

    explicit vector(size_t size, const T &x = T());

    vector(const vector &v);

    vector &operator=(const vector &v);

    vector(vector &&v) noexcept;

    vector &operator=(vector &&v) noexcept;

    vector(std::initializer_list<T> init);

    template<typename InputIt>
    explicit vector(InputIt first, InputIt last);

    reference operator[](size_type idx);

    const_reference operator[](size_type idx) const;

    reference front();

    const_reference front() const;

    reference back();

    const_reference back() const;

    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();

    const_iterator end() const;

    const_iterator cend() const;

    size_type size() const { return _size; }

    size_type capacity() const { return _capacity; }

    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    bool empty() const { return _size == 0; }

    void shrink_to_fit();

    void push_back(const T &value);

    void push_back(T &&value);

    template<class... Args>
    reference emplace_back(Args &&... args);

    void pop_back();

    void clear() noexcept;

    void reserve(size_t new_cap) {return reserve_internal(new_cap);}

    void swap(vector& o) noexcept;

};

template<typename T, typename Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::_allocator;

template<typename T, typename Allocator>
vector<T, Allocator>::vector() : _size(0), _capacity(0), _data(nullptr) {

}

template<typename T, typename Allocator>
vector<T, Allocator>::~vector() {
    std::destroy(_data,_data+_size);
    if (_data)_allocator.deallocate(_data, _capacity);
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(size_t size, const T &x) : _size(size), _capacity(size),
                                                        _data(_allocator.allocate(size)) {
    std::fill(_data, _data + size, x);
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(const vector &v) : _size(v._size), _capacity(v._size),
                                                _data(_allocator.allocate(v._size)) {
    std::copy(v._data, v._data + v._size, _data);
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(vector &&v) noexcept : _size(v._size), _capacity(v._capacity), _data(v._data) {
    v._size = v._capacity = 0;
    v._data = nullptr;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](vector::size_type idx) {
    return _data[idx];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](vector::size_type idx) const {
    return _data[idx];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
    return *_data;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
    return *_data;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
    return _data[_size - 1];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
    return _data[_size - 1];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() {
    return _data;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const {
    return _data;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const {
    return _data;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() {
    return _data + _size;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const {
    return _data + _size;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const {
    return _data + _size;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::reserve_internal(size_t nsize) {
    if (nsize <= _capacity)return;
    pointer ndata = _allocator.allocate(nsize);
    if (_capacity) {
        std::uninitialized_move(_data, _data + _size, ndata);
        std::destroy(_data,_data+_size);
        _allocator.deallocate(_data, _capacity);
    }
    _data = ndata;
    _capacity = nsize;
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size()),
                                                              _data(_allocator.allocate(init.size())) {
    std::copy(init.begin(), init.end(), _data);
}

template<typename T, typename Allocator>
template<typename InputIt>
vector<T, Allocator>::vector(InputIt first, InputIt last) : _size(std::distance(first, last)), _capacity(_size),
                                                            _data(_allocator.allocate(_size)) {
    std::copy(first, last, _data);
}

template<typename T, typename Allocator>
void vector<T, Allocator>::shrink_to_fit() {
    if (_capacity == _size)return;
    if (_size) {
        pointer ndata = _allocator.allocate(_size);
        std::uninitialized_move(_data, _data + _size, ndata);
    }
    _allocator.deallocate(_data, _capacity);
    _capacity = _size;

}

template<typename T, typename Allocator>
void vector<T, Allocator>::push_back(const T &value) {
    reserve_for_insertion();
    _allocator.construct(_data + _size,value);
    ++_size;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::push_back(T &&value) {
    reserve_for_insertion();
    _allocator.construct(_data + _size,std::move(value));
    ++_size;
}

template<typename T, typename Allocator>
template<class... Args>
typename vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&... args) {
    reserve_for_insertion();
    _allocator.construct(_data+_size,std::forward<Args>(args)...);
    return _data[_size++];
}

template<typename T, typename Allocator>
void vector<T, Allocator>::pop_back() {
    --_size;
}

template<typename T, typename Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &v) {
    reserve_internal(v._size);
    _size = v._size;
    std::copy(v._data, v._data + _size, v._data);
    return *this;
}

template<typename T, typename Allocator>
vector<T,Allocator> &vector<T, Allocator>::operator=(vector &&v) noexcept {
    swap(v);
    return *this;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::swap(vector &o) noexcept {
    std::swap(_data,o._data);
    std::swap(_size,o._size);
    std::swap(_capacity,o._capacity);
}

template<typename T, typename Allocator>
void vector<T, Allocator>::clear() noexcept {
    std::destroy(_data,_data+_size);
    _size=0;
}


}
#endif //ALG_VECTOR_H

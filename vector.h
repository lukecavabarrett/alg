#ifndef ALG_VECTOR_H
#define ALG_VECTOR_H

#include <cstddef>

namespace alg {

    template<typename T, typename Allocator = std::allocator<T> >
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef pointer iterator;
        typedef const_pointer const_iterator;
    private:
        allocator_type _allocator;
        size_t _size, _capacity;
        T *_data;
        void reserve_internal(size_t nsize);
    public:
        vector();

        ~vector();

        explicit vector(size_t size, const T &x = T());

        vector(const vector &v);

        vector(vector &&v) noexcept;

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
    };

    template<typename T, typename Allocator>
    vector<T, Allocator>::vector() : _size(0), _capacity(0), _data(nullptr) {

    }

    template<typename T, typename Allocator>
    vector<T, Allocator>::~vector() {
        if (_data)_allocator.deallocate(_data,_capacity);
    }

    template<typename T, typename Allocator>
    vector<T, Allocator>::vector(size_t size, const T &x) : _size(size), _capacity(size), _data(_allocator.allocate(size)) {
        std::fill(_data, _data + size, x);
    }

    template<typename T, typename Allocator>
    vector<T, Allocator>::vector(const vector &v) : _size(v._size), _capacity(v._size), _data(_allocator.allocate(v._size)) {
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
    

}
#endif //ALG_VECTOR_H

#pragma once

#include <iterator>


template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using difference = std::ptrdiff_t;

private:
    pointer ptr_;

public:
    Iterator(pointer ptr)
        : ptr_(ptr)
    {}


    operator pointer() {
        return ptr_;
    }
    operator const_pointer() const {
        return ptr_;
    }

    reference operator* () {
        return *ptr_;
    }
    const_reference operator* () const {
        return *ptr_;
    }

    reference operator[] (difference n) {
        return *(ptr_ + n);
    }
    const_reference operator[] (difference n) const {
        return *(ptr_ + n);
    }


    iterator& operator++() {
        ptr_++;
        return *this;
    }
    iterator operator++(int) {
        iterator temp(ptr_);
        ptr_++;
        return temp;
    }
    iterator& operator--() {
        ptr_--;
        return *this;
    }
    iterator operator--(int) {
        iterator temp(ptr_);
        ptr_--;
        return temp;
    }


    iterator& operator+= (difference n) {
        ptr_ += n;
        return *this;
    }
    iterator& operator-= (difference n) {
        ptr_ -= n;
        return *this;
    }
    iterator operator+ (difference n) const {
        return iterator(ptr_ + n);
    }
    iterator operator- (difference n) const {
        return iterator(ptr_ - n);
    }
    template<class U>
    friend Iterator<U> operator+ (typename Iterator<U>::difference, const Iterator<U>&);

    iterator::difference operator- (const iterator& it) const {
        return ptr_ - it.ptr_;
    }


    bool operator== (const iterator& it) const {
        return ptr_ == it.ptr_;
    }
    bool operator!= (const iterator& it) const {
        return ptr_ != it.ptr_;
    }
    bool operator< (const iterator& it) const {
        return ptr_ < it.ptr_;
    }
    bool operator> (const iterator& it) const {
        return ptr_ > it.ptr_;
    }
    bool operator<= (const iterator& it) const {
        return ptr_ <= it.ptr_;
    }
    bool operator>= (const iterator& it) const {
        return ptr_ >= it.ptr_;
    }
};

template<class U>
Iterator<U> operator+ (typename Iterator<U>::difference n, const Iterator<U>& it) {
    return iterator(it.ptr_ + n);
}

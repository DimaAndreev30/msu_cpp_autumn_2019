#pragma once

#include <limits>


template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    Allocator noexept() {}
    ~Allocator() {}

    pointer allocate(size_type);
    void deallocate(pointer, size_type);

    template<class... Args>
    void construct(pointer, Args&&...);
    void construct(pointer, const_reference);
    void construct(pointer, T&&);
    void destroy(pointer);

    size_type max_size();
};


template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(operator new [](n*sizeof(value_type)));
}

template<class T>
void Allocator<T>::deallocate(pointer ptr, size_type n)
{
    operator delete[](ptr, n*sizeof(value_type));
}


template<class T>
template<class... Args>
void Allocator<T>::construct(pointer ptr, Args&&... args)
{
    new(ptr) value_type(std::forward<Args>(args)...);
}

template<class T>
void Allocator<T>::construct(pointer ptr, const_reference value)
{
    new(ptr) value_type(value);
}

template<class T>
void Allocator<T>::construct(pointer ptr, T&& value)
{
    new(ptr) value_type(value);
}

template<class T>
void Allocator<T>::destroy(pointer ptr)
{
    static_cast<pointer>(ptr)->~T();
}


template<class T>
typename Allocator<T>::size_type Allocator<T>::max_size()
{
    return std::numeric_limits<size_type>::max();
}

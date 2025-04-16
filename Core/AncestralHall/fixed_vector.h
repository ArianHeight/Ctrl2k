#pragma once
#include <utility>
#include "Core/Monument/basicassert.h"

namespace ahl
{

/*

A vector allocated on the stack with a fixed max capacity.

*/

template<typename T, size_t capacity>
class fixed_vector
{
private:
    using selftype = fixed_vector<T, capacity>;

    T data[capacity];
    size_t size;

public:
    fixed_vector() : size(0) {}
    
    inline size_t size() const { return size; }
    inline size_t capacity() const { return capacity; }
    inline bool empty() const { return size == 0; }

    inline void clear() { size = 0; }
    inline void pop_back() { size--; }

    void push_back(const T& elem)
    {
        data[size] = elem;
        size++;
    }

    void push_back(T&& elem)
    {
        data[size] = std::move(elem);
        size++;
    }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < other.size; i++)
            {
                data[i] = other.data[i];
            }
            size = other.size;
        }
        return *this;
    }
    //TODO make this compatible with all other fixed_vector types

    inline const T& at(size_t i) const { return data[i]; }
    inline T& at(size_t i) { return data[i]; }
    inline const T& operator[](size_t i) const { return data[i]; }
    inline T& operator[](size_t i) { return data[i]; }
    inline const T* data() const { return data; }
    inline T* data() { return data; }
    inline const T& front() const { return data[0]; }
    inline T& front() { return data[0]; }
    inline const T& back() const { return data[size - 1]; }
    inline T& back() { return data[size - 1]; }
};

}
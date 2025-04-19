#pragma once
#include <utility>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

A vector allocated on the stack with a fixed max capacity.

*/

template<typename T, size_t _capacity>
class fixed_vector
{
private:
    using selftype = fixed_vector<T, _capacity>;

    size_t m_size;
    T m_data[_capacity];
    
public:
    fixed_vector() : m_size(0) {}
    
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }
    inline void pop_back() { assert(m_size > 0); m_size--; }

    void push_back(const T& elem)
    {
        assert(m_size < capacity);
        m_data[m_size] = elem;
        m_size++;
    }

    void push_back(T&& elem)
    {
        assert(m_size < _capacity);
        m_data[m_size] = std::move(elem);
        m_size++;
    }

    T& emplace_back()
    {
        assert(m_size < _capacity);
        T& retVal = m_data[m_size];
        m_size++;
        return retVal;
    }

    //TODO maybe use memcpy??
    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < other.m_size; i++)
            {
                m_data[i] = other.m_data[i];
            }
            m_size = other.m_size;
        }
        return *this;
    }
    //TODO make this compatible with all other fixed_vector types

    inline const T& at(size_t i) const { return m_data[i]; }
    inline T& at(size_t i) { return m_data[i]; }
    inline const T& operator[](size_t i) const { return m_data[i]; }
    inline T& operator[](size_t i) { return m_data[i]; }
    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }
    inline const T& front() const { return m_data[0]; }
    inline T& front() { return m_data[0]; }
    inline const T& back() const { return m_data[m_size - 1]; }
    inline T& back() { return m_data[m_size - 1]; }
};

}
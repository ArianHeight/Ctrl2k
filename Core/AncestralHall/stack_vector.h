#pragma once
#include <utility>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

A vector allocated on the stack with a fixed max capacity.

*/

template <typename T, size_t _capacity>
class stack_vector
{
    template <typename T2, size_t _capacity2>
    friend class stack_vector;

private:
    using selftype = stack_vector<T, _capacity>;

    size_t m_size;
    T m_data[_capacity];
    
public:
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }
    inline void pop_back() { assert(m_size > 0); m_size--; }

    void push_back(const T& elem)
    {
        assert(m_size < _capacity);
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
        T& retval = m_data[m_size];
        m_size++;
        return retval;
    }

    void resize(size_t new_size, const T& default_elem = {})
    {
        assert(new_size <= _capacity);
        for(; m_size < new_size; ++m_size)
        {
            m_data[m_size] = default_elem;
        }
        m_size = new_size;
    }

    void insert(size_t index, const T& elem)
    {
        assert(m_size < _capacity&& index <= m_size);
        const size_t count = m_size - index;
        for(size_t i = 0; i < count; ++i)
        {
            m_data[m_size - i] = std::move(m_data[m_size - i - 1]);
        }
        m_data[index] = elem;
        ++m_size;
    }

    void insert(size_t index, T&& elem)
    {
        assert(m_size < _capacity && index <= m_size);
        const size_t count = m_size - index;
        for(size_t i = 0; i < count; ++i)
        {
            m_data[m_size - i] = std::move(m_data[m_size - i - 1]);
        }
        m_data[index] = std::move(elem);
        ++m_size;
    }

    void erase(size_t index)
    {
        assert(m_size > 0 && index < m_size);
        --m_size;
        for(index; index < m_size; ++index)
        {
            m_data[index] = std::move(m_data[index + 1]);
        }
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

    selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < other.m_size; i++)
            {
                m_data[i] = std::move(other.m_data[i]);
            }
            m_size = other.m_size;
            other.m_size = 0;
        }
        return *this;
    }

    template<size_t _other_capacity>
    selftype& operator=(const stack_vector<T, _other_capacity>& other)
    {
        assert(other.size() <= _capacity);
        for(size_t i = 0; i < other.m_size; i++)
        {
            m_data[i] = other.m_data[i];
        }
        m_size = other.m_size;
        return *this;
    }

    template<size_t _other_capacity>
    selftype& operator=(stack_vector<T, _other_capacity>&& other)
    {
        assert(other.size() <= _capacity);
        for(size_t i = 0; i < other.m_size; i++)
        {
            m_data[i] = std::move(other.m_data[i]);
        }
        m_size = other.m_size;
        other.m_size = 0;
        return *this;
    }

    // cstrs
    stack_vector() : m_size(0) {}
    stack_vector(const selftype& other) { *this = other; }
    stack_vector(selftype&& other) { *this = std::move(other); }
    template<size_t _other_capacity>
    stack_vector(const stack_vector<T, _other_capacity>& other) : m_size(0) { *this = other; }
    template<size_t _other_capacity>
    stack_vector(stack_vector<T, _other_capacity>&& other) : m_size(0) { *this = std::move(other); }

    template<size_t _other_capacity>
    bool operator==(const stack_vector<T, _other_capacity>& other) const
    {
        if(m_size != other.m_size)
            return false;

        for(size_t i = 0; i < m_size; ++i)
        {
            if(m_data[i] != other.m_data[i])
            {
                return false;
            }
        }

        return true;
    }

    inline const T& at(size_t i) const { index_assert(i, m_size); return m_data[i]; }
    inline T& at(size_t i) { index_assert(i, m_size); return m_data[i]; }
    inline const T& operator[](size_t i) const { return at(i); }
    inline T& operator[](size_t i) { return at(i); }
    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }
    inline const T& front() const { assert(m_size > 0); return m_data[0]; }
    inline T& front() { assert(m_size > 0); return m_data[0]; }
    inline const T& back() const { assert(m_size > 0); return m_data[m_size - 1]; }
    inline T& back() { assert(m_size > 0); return m_data[m_size - 1]; }
};

}
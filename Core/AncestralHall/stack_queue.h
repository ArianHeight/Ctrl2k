#pragma once
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

This is an implementation of a dequeue on the stack, with a fixed max capacity.

Requires a size of a power of 2(so we don't need to use mod)

*/

template <typename T, size_t _capacity>
requires (_capacity > 0 && is_exp_of_two(_capacity)) // needed to use bitmasking instead of modulus
class stack_dequeue
{
    template <typename T2, size_t _capacity2>
    requires (_capacity2 > 0 && is_exp_of_two(_capacity2))
    friend class stack_dequeue;

private:
    using selftype = stack_dequeue<T, _capacity>;

    static constexpr size_t _index_mask = _capacity - 1;

    T m_data[_capacity];
    size_t m_size;
    size_t m_head;

public:
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }
    inline const T& peek_front() const { assert(m_size > 0); return m_data[m_head]; }
    inline const T& peek_back() const { assert(m_size > 0); return m_data[(m_head + m_size - 1) & _index_mask]; }
    inline void pop_front() { assert(m_size > 0); m_head = (m_head + 1) & _index_mask; --m_size; }
    inline void pop_back() { assert(m_size > 0); --m_size; }

    void push_back(const T& elem)
    {
        assert(m_size < _capacity);
        m_data[(m_head + m_size) & _index_mask] = elem;
        ++m_size;
    }

    void push_back(T&& elem)
    {
        assert(m_size < _capacity);
        m_data[(m_head + m_size) & _index_mask] = std::move(elem);
        ++m_size;
    }

    T& emplace_back()
    {
        assert(m_size < _capacity);
        T& retval = m_data[(m_head + m_size) & _index_mask];
        ++m_size;
        return retval;
    }

    void push_front(const T& elem)
    {
        assert(m_size < _capacity);
        m_head = (m_head - 1) & _index_mask;
        m_data[m_head] = elem;
        ++m_size;
    }

    void push_front(T&& elem)
    {
        assert(m_size < _capacity);
        m_head = (m_head - 1) & _index_mask;
        m_data[m_head] = std::move(elem);
        ++m_size;
    }

    T& emplace_front()
    {
        assert(m_size < _capacity);
        m_head = (m_head - 1) & _index_mask;
        T& retval = m_data[m_head];
        ++m_size;
        return retval;
    }

    inline size_t begin_index() const { return m_head; }

    size_t next_index(size_t index) const
    {
        size_t next = (index + 1) & _index_mask;
        return next != ((m_head + m_size) & _index_mask) ? next : INVALID_SIZE_T;
    }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < other.m_size; ++i)
            {
                m_data[i] = other.m_data[(other.m_head + i) & _index_mask];
            }
            m_head = 0;
            m_size = other.m_size;
        }
        return *this;
    }

    selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < other.m_size; ++i)
            {
                m_data[i] = std::move(other.m_data[(other.m_head + i) & _index_mask]);
            }
            m_head = 0;
            m_size = other.m_size;
            other.m_head = 0;
            other.m_size = 0;
        }
        return *this;
    }

    template<size_t _other_capacity>
    selftype& operator=(const stack_dequeue<T, _other_capacity>& other)
    {
        assert(other.size() <= _capacity);
        for(size_t i = 0; i < other.m_size; ++i)
        {
            m_data[i] = other.m_data[(other.m_head + i) & other._index_mask];
        }
        m_head = 0;
        m_size = other.m_size;
        return *this;
    }

    template<size_t _other_capacity>
    selftype& operator=(stack_dequeue<T, _other_capacity>&& other)
    {
        assert(other.size() <= _capacity);
        for(size_t i = 0; i < other.m_size; ++i)
        {
            m_data[i] = std::move(other.m_data[(other.m_head + i) & other._index_mask]);
        }
        m_head = 0;
        m_size = other.m_size;
        other.m_head = 0;
        other.m_size = 0;
        return *this;
    }

    // cstrs
    stack_dequeue() : m_head(0), m_size(0) {}
    stack_dequeue(const selftype& other) { *this = other; }
    stack_dequeue(selftype&& other) { *this = std::move(other); }
    template<size_t _other_capacity>
    stack_dequeue(const stack_dequeue<T, _other_capacity>& other) { *this = other; }
    template<size_t _other_capacity>
    stack_dequeue(stack_dequeue<T, _other_capacity>&& other) { *this = std::move(other); }

    template<size_t _other_capacity>
    bool operator==(const stack_dequeue<T, _other_capacity>& other) const
    {
        if(m_size != other.m_size)
            return false;

        for(size_t i = 0; i < m_size; ++i)
        {
            if(m_data[(m_head + i) & _index_mask] != other.m_data[(other.m_head + i) & other._index_mask])
            {
                return false;
            }
        }

        return true;
    }

    inline const T& at(size_t i) const { index_assert(i, m_size); return m_data[(m_head + i) & _index_mask]; }
    inline T& at(size_t i) { index_assert(i, m_size); return m_data[(m_head + i) & _index_mask]; }
    inline const T& operator[](size_t i) const { return at(i); }
    inline T& operator[](size_t i) { return at(i); }
};

}

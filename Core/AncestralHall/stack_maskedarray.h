#pragma once
#include "stack_bitvector.h"

namespace ahl
{

/*

This is not a typical vector with a push_back functionality. Instead, valid elements are marked as valid in a separate bit vector.

*/

template <typename T, size_t _capacity>
class stack_maskedarray
{
private:
    using selftype = stack_maskedarray<T, _capacity>;

    T m_data[_capacity];
    stack_bitvector<_capacity> m_filled;
    size_t m_size;

public:
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }
    inline bool is_filled(size_t index) const { index_assert(index, _capacity); return m_filled[index]; }

    inline void clear() { m_filled.set_all_false(); m_size = 0; }

    bool fill(size_t index, const T& val)
    {
        index_assert(index, _capacity);
        if(m_filled[index])
            return false;
        m_data[index] = val;
        m_filled.set_bit(index, true);
        ++m_size;
        return true;
    }

    bool fill(size_t index, T&& val)
    {
        index_assert(index, _capacity);
        if(m_filled[index])
            return false;
        m_data[index] = std::move(val);
        m_filled.set_bit(index, true);
        ++m_size;
        return true;
    }

    void fill_or_overwrite(size_t index, const T& val)
    {
        index_assert(index, _capacity);
        m_data[index] = val;
        m_size += m_filled[index] ? 0 : 1;
        m_filled.set_bit(index, true);
    }

    void fill_or_overwrite(size_t index, T&& val)
    {
        index_assert(index, _capacity);
        m_data[index] = std::move(val);
        m_size += m_filled[index] ? 0 : 1;
        m_filled.set_bit(index, true);
    }

    void erase(size_t index)
    {
        index_assert(index, _capacity);
        if(m_filled[index])
        {
            m_filled.set_bit(index, false);
            --m_size;
        }
    }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < _capacity; ++i)
            {
                if(other.m_filled[i])
                {
                    m_data[i] = other.m_data[i];
                }
            }
            m_size = other.m_size;
            m_filled = other.m_filled;
        }
        return *this;
    }

    selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < _capacity; ++i)
            {
                if(other.m_filled[i])
                {
                    m_data[i] = std::move(other.m_data[i]);
                }
            }
            m_size = other.m_size;
            m_filled = std::move(other.m_filled);
            other.m_size = 0;
        }
        return *this;
    }

    // cstrs
    stack_maskedarray() : m_size(0)
    {
        m_filled.resize(_capacity);
    }

    stack_maskedarray(const selftype& other) : m_filled(other.m_filled), m_size(other.m_size)
    {
        for(size_t i = 0; i < _capacity; ++i)
        {
            if(m_filled[i])
            {
                m_data[i] = other.m_data[i];
            }
        }
    }

    stack_maskedarray(selftype&& other) : m_filled(std::move(other.m_filled)), m_size(other.m_size)
    {
        for(size_t i = 0; i < _capacity; ++i)
        {
            if(m_filled[i])
            {
                m_data[i] = std::move(other.m_data[i]);
            }
        }
        other.m_size = 0;
    }

    bool operator==(const selftype& other) const
    {
        if(m_size != other.m_size || m_filled != other.m_filled)
            return false;
        for(size_t i = 0; i < _capacity; ++i)
        {
            if(m_filled[i] && m_data[i] != other.m_data[i])
            {
                return false;
            }
        }
        return true;
    }

    inline const T& at(size_t index) const { index_assert(index, _capacity); assert(m_filled[index]); return m_data[index]; }
    inline T& at(size_t index) { index_assert(index, _capacity); assert(m_filled[index]); return m_data[index]; }
    inline const T& operator[](size_t index) const { return at(index); }
    inline T& operator[](size_t index) { return at(index); }
};

}
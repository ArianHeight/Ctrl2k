#pragma once
#include "stack_maskedarray.h"

namespace ahl
{

/*

This is a set implementation that uses hashing instead of sorting.
This uses stack_vector as the underlying data container, so it has a max capacity.

Because this uses hashing, collisions should be avoided if possible by having a capacity around 2x what is needed.

*/

template<typename T, size_t _capacity, class _hash_functor = _generic_hash_functor<T>>
requires (_capacity > 0 && is_exp_of_two(_capacity)) // needed to use bitmasking instead of modulus
class stack_hashset
{
private:
    using selftype = stack_hashset<T, _capacity, _hash_functor>;

    static constexpr size_t _max_load_factor = (size_t)(0.75f * _capacity);
    static constexpr size_t _index_mask = _capacity - 1;

    stack_maskedarray<T, _capacity> m_data;
    _hash_functor hash;

    inline size_t find_insert_pos(const T& val) const
    {
        size_t index = hash(val).val & _index_mask;
        for(; m_data.is_filled(index) && m_data[index] != val; index = (index + 1) & _index_mask);
        return index;
    }

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }
    inline bool max_load_factor_exceeded() const { return m_data.size() >= _max_load_factor; }

    inline void clear() { m_data.clear(); }

    size_t find_index(const T& val) const
    {
        size_t index = hash(val).val & _index_mask;
        for(; m_data.is_filled(index) && m_data[index] != val; index = (index + 1) & _index_mask);
        index = (m_data.is_filled(index) && m_data[index] == val) ? index : INVALID_SIZE_T;
        return index;
    }

    inline bool contains(const T& val) const { return find_index(val) != INVALID_SIZE_T; }

    size_t insert(const T& val)
    {
        assert(m_data.size() < m_data.capacity());
        size_t index = find_insert_pos(val);
        if(m_data.is_filled(index))
            return INVALID_SIZE_T;
        m_data.fill(index, val);
        return index;
    }

    size_t insert(T&& val)
    {
        assert(m_data.size() < m_data.capacity());
        size_t index = find_insert_pos(val);
        if(m_data.is_filled(index))
            return INVALID_SIZE_T;
        m_data.fill(index, std::move(val));
        return index;
    }

    size_t erase(const T& val)
    {
        size_t index = find_index(val);
        if(index != INVALID_SIZE_T)
            m_data.erase(index);
        return index;
    }

    // TODO this isn't actually so simple. we need to compare element-wise instead since different order insertions
    // will cause different orderings in case of hash conflicts
    inline selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            m_data = other.m_data;
        }
        return *this;
    }

    inline selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    // cstrs
    stack_hashset() = default;
    stack_hashset(const selftype& other) : m_data(other.m_data) {}
    stack_hashset(selftype&& other) : m_data(std::move(other.m_data)) {}

    inline bool operator==(const selftype& other) const
    {
        return m_data == other.m_data;
    }

    inline const T& at_index(size_t i) const { return m_data[i]; }
};

}
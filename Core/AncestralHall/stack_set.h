#pragma once
#include "stack_vector.h"

namespace ahl
{

/*

This works like std::set, except instead of using a BST, it's just a sorted vector. This means the insertion and deletion is O(n)
and not O(log(n))
Uses a stack_vector as the underlying data container, so this type has a defined max capacity

*/

template <typename T, size_t _capacity, class _compare_functor = _sort_compare_functor<T>>
class stack_set
{
    template <typename T2, size_t _capacity2, class _compare_functor2>
    friend class stack_set;

private:
    using selftype = stack_set<T, _capacity, _compare_functor>;

    stack_vector<T, _capacity> m_data;

    inline size_t find_insert_pos(const T& val) const
    {
        return binary_search_position(m_data.data(), m_data.size(), val, _compare_functor());
    }

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }

    inline void clear() { m_data.clear(); }

    size_t find_index(const T& val) const
    {
        return binary_search(m_data.data(), m_data.size(), val, _compare_functor());
    }

    inline bool contains(const T& val) const { return find_index(val) != INVALID_SIZE_T; }

    size_t insert(const T& val)
    {
        size_t index = find_insert_pos(val);
        if(index >= m_data.size())
            m_data.push_back(val);
        else if(m_data[index] != val)
            m_data.insert(index, val);
        return index;
    }

    size_t insert(T&& val)
    {
        size_t index = find_insert_pos(val);
        if(index >= m_data.size())
            m_data.push_back(std::move(val));
        else if(m_data[index] != val)
            m_data.insert(index, std::move(val));
        return index;
    }

    void erase(const T& val)
    {
        const size_t index = find_index(val);
        if(index != INVALID_SIZE_T)
            m_data.erase(index);
    }

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

    template <size_t _other_capacity>
    inline selftype& operator=(const stack_set<T, _other_capacity, _compare_functor>& other)
    {
        m_data = other.m_data;
        return *this;
    }

    template <size_t _other_capacity>
    inline selftype& operator=(stack_set<T, _other_capacity, _compare_functor>&& other)
    {
        m_data = std::move(other.m_data);
        return *this;
    }

    // cstrs
    stack_set() = default;
    template <size_t _other_capacity>
    stack_set(const stack_set<T, _other_capacity, _compare_functor>& other) : m_data(other.m_data) {}
    template <size_t _other_capacity>
    stack_set(stack_set<T, _other_capacity, _compare_functor>&& other) : m_data(std::move(other.m_data)) {}

    template <size_t _other_capacity>
    inline bool operator==(const stack_set<T, _other_capacity, _compare_functor>& other) const
    {
        return m_data == other.m_data;
    }

    inline const T& at_index(size_t i) const { return m_data[i]; }
};

}
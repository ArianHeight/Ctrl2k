#pragma once
#include "stack_vector.h"

namespace ahl
{

/*

This works like std::set, except instead of using a BST, it's just a sorted vector. This means the insertion and deletion is O(n)
and not O(log(n))

*/

template <typename T, size_t _capacity, class _compare_functor = _sort_compare_functor<T>>
class stack_set
{
    template <typename T2, size_t _capacity2, class _compare_functor2>
    friend class stack_set;

private:
    using selftype = stack_set<T, _capacity, _compare_functor>;

    stack_vector<T, _capacity> m_data;

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }

    inline void clear() { m_data.clear(); }

    bool contains(const T& val) const
    {
        return binary_search(m_data.data(), m_data.size(), val, _compare_functor()) != INVALID_SIZE_T;
    }

    void insert(const T& val)
    {
        const size_t index = binary_search_position(m_data.data(), m_data.size(), val, _compare_functor());
        if(index >= m_data.size())
            m_data.push_back(val);
        else if(m_data[index] != val)
            m_data.insert(index, val);
    }

    void insert(T&& val)
    {
        const size_t index = binary_search_position(m_data.data(), m_data.size(), val, _compare_functor());
        if(index >= m_data.size())
            m_data.push_back(std::move(val));
        else if(m_data[index] != val)
            m_data.insert(index, std::move(val));
    }

    void erase(const T& val)
    {
        const size_t index = binary_search(m_data.data(), m_data.size(), val, _compare_functor());
        if(index != INVALID_SIZE_T)
            m_data.erase(index);
    }
    
    inline selftype& operator=(const selftype& other)
    {
        m_data = other.m_data;
        return *this;
    }

    inline selftype& operator=(selftype&& other)
    {
        m_data = std::move(other.m_data);
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
    stack_set(const selftype& other) { m_data = other.m_data; }
    stack_set(selftype&& other) { m_data = std::move(other.m_data); }
    template <size_t _other_capacity>
    stack_set(const stack_set<T, _other_capacity, _compare_functor>& other) { m_data = other.m_data; }
    template <size_t _other_capacity>
    stack_set(stack_set<T, _other_capacity, _compare_functor>&& other) { m_data = std::move(other.m_data); }

    template <size_t _other_capacity>
    inline bool operator==(const stack_set<T, _other_capacity, _compare_functor>& other) const
    {
        return m_data == other.m_data;
    }

    inline const T& at(size_t i) const { return m_data[i]; }
    inline const T& operator[](size_t i) const { return m_data[i]; }
};

}
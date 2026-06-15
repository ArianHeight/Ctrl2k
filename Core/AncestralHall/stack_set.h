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
    stack_vector<T, _capacity> m_data;

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }

    inline void clear() { m_data.clear(); }

    // TODO we should do binary search to nearest and directly insert instead of calling insertion sort
    void insert(const T& val)
    {
        m_data.push_back(val);
        insertion_sort(m_data.data(), _compare_functor);
    }

    void insert(T&& val)
    {
        m_data.push_back(std::move(val));
        insertion_sort(m_data.data(), _compare_functor);
    }

    // TODO finish this
};

}
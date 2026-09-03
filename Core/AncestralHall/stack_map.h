#pragma once
#include "stack_vector.h"

namespace ahl
{

/*

This is a map implementation using stack_set as the underlying data container, so this type has a defined max capacity

*/

template <typename K, typename V, size_t _capacity, class _compare_functor = _sort_compare_functor<K>>
class stack_map
{
    template <typename K2, typename V2, size_t _capacity2, class _compare_functor2>
    friend class stack_map;

private:
    using selftype = stack_map<K, V, _capacity, _compare_functor>;
    using kvptype = key_value_pair<K, V>;
    using kvpref = key_value_pair_ref<K, V>;
    using kvpcref = key_value_pair_const_ref<K, V>;
    using kvpcompare = _kvp_key_compare_functor<K, V, _compare_functor>;

    stack_vector<kvptype, _capacity> m_data;

    inline size_t find_insert_pos(const K& key) const
    {
        return binary_search_key_position(m_data.data(), m_data.size(), key, kvpcompare());
    }

    // need to do this for forwarding reference
    template <typename P, typename U>
    inline size_t insert_internal(P&& key, U&& val)
    {
        size_t index = find_insert_pos(key);
        if(index >= m_data.size())
            m_data.push_back({ std::forward<P>(key), std::forward<U>(val) });
        else if(m_data[index].key != key)
            m_data.insert(index, { std::forward<P>(key), std::forward<U>(val) });
        else
            return INVALID_SIZE_T;
        return index;
    }

    // need to do this for forwarding reference
    template <typename P, typename U>
    inline size_t insert_or_assign_internal(P&& key, U&& val)
    {
        size_t index = find_insert_pos(key);
        if(index >= m_data.size())
            m_data.push_back({ std::forward<P>(key), std::forward<U>(val) });
        else if(m_data[index].key != key)
            m_data.insert(index, { std::forward<P>(key), std::forward<U>(val) });
        else
            m_data[index].value = std::forward<U>(val);
        return index;
    }

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }

    inline void clear() { m_data.clear(); }

    size_t find_index(const K& key) const
    {
        return binary_search_key(m_data.data(), m_data.size(), key, kvpcompare());
    }

    inline bool contains(const K& key) const { return find_index(key) != INVALID_SIZE_T; }

    size_t insert(const K& key, const V& val) { return insert_internal(key, val); }
    size_t insert(const K& key, V&& val) { return insert_internal(key, std::move(val)); }
    size_t insert(K&& key, V&& val) { return insert_internal(std::move(key), std::move(val)); }
    size_t insert(K&& key, const V& val) { return insert_internal(std::move(key), val); }

    size_t insert_or_assign(const K& key, const V& val) { return insert_or_assign_internal(key, val); }
    size_t insert_or_assign(const K& key, V&& val) { return insert_or_assign_internal(key, std::move(val)); }
    size_t insert_or_assign(K&& key, V&& val) { return insert_or_assign_internal(std::move(key), std::move(val)); }
    size_t insert_or_assign(K&& key, const V& val) { return insert_or_assign_internal(std::move(key), val); }

    // return the index of the next item, this is to facilitate deletion while iterating
    size_t erase(const K& key)
    {
        size_t index = find_index(key);
        if(index != INVALID_SIZE_T)
        {
            m_data.erase(index);
        }
        return index >= m_data.size() ? INVALID_SIZE_T : index;
    }

    template <size_t _other_capacity>
    inline selftype& operator=(const stack_map<K, V, _other_capacity, _compare_functor>& other)
    {
        m_data = other.m_data;
        return *this;
    }

    template <size_t _other_capacity>
    inline selftype& operator=(stack_map<K, V, _other_capacity, _compare_functor>&& other)
    {
        m_data = std::move(other.m_data);
        return *this;
    }

    // cstrs
    stack_map() = default;
    template <size_t _other_capacity>
    stack_map(const stack_map<K, V, _other_capacity, _compare_functor>&other) : m_data(other.m_data) {}
    template <size_t _other_capacity>
    stack_map(stack_map<K, V, _other_capacity, _compare_functor> && other) : m_data(std::move(other.m_data)) {}
    
    template <size_t _other_capacity>
    inline bool operator==(const stack_map<K, V, _other_capacity, _compare_functor>& other) const
    {
        return m_data == other.m_data;
    }

    inline kvpcref at_index(size_t i) const { return { m_data[i].key, m_data[i].value }; }
    inline kvpref at_index(size_t i) { return { m_data[i].key, m_data[i].value }; }
    const V& at(const K& key) const
    {
        const size_t index = find_index(key);
        assert(index != INVALID_SIZE_T);
        return m_data[index].value;
    }
    inline const V& operator[](const K& key) const { return at(key); }
    V& at(const K& key)
    {
        const size_t index = find_index(key);
        assert(index != INVALID_SIZE_T);
        return m_data[index].value;
    }
    inline V& operator[](const K& key) { return at(key); }
};

}
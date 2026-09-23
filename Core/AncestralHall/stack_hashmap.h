#pragma once
#include "stack_maskedarray.h"

namespace ahl
{

/*

This is a map implementation that uses hashing instead of sorting.
This uses stack_maskedarray as the underlying data container, so it has a max capacity.

Because this uses hashing, collisions should be avoided if possible by having a capacity around 2x what is needed.

*/

template <typename K, typename V, size_t _capacity, class _hash_functor = _generic_hash_functor<K>>
requires (_capacity > 0 && is_exp_of_two(_capacity)) // needed to use bitmasking instead of modulus
class stack_hashmap
{
    template <typename K2, typename V2, size_t _capacity2, class _hash_functor2>
    requires (_capacity2 > 0 && is_exp_of_two(_capacity2))
    friend class stack_hashmap;

public:
    using kvptype = key_value_pair<K, V>;
    using kvpref = key_value_pair_ref<K, V>;
    using kvpcref = key_value_pair_const_ref<K, V>;

private:
    using selftype = stack_hashmap<K, V, _capacity, _hash_functor>;

    static constexpr size_t _max_load_factor = (size_t)(0.75f * _capacity);
    static constexpr size_t _index_mask = _capacity - 1;

    stack_maskedarray<kvptype, _capacity> m_data;
    _hash_functor hash;

    inline size_t find_insert_pos(const K& key) const
    {
        size_t index = hash(key).val & _index_mask;
        for(; m_data.is_filled(index) && m_data[index].key != key; index = (index + 1) & _index_mask);
        return index;
    }

    // need to do this for forwarding reference
    template <typename P, typename U>
    inline size_t insert_internal(P&& key, U&& val)
    {
        assert(m_data.size() < _max_load_factor);
        size_t index = find_insert_pos(key);
        if(!m_data.is_filled(index))
            m_data.fill(index, { std::forward<P>(key), std::forward<U>(val) });
        else
            return INVALID_SIZE_T;
        return index;
    }

    // need to do this for forwarding reference
    template <typename P, typename U>
    inline size_t insert_or_assign_internal(P&& key, U&& val)
    {
        assert(m_data.size() < _max_load_factor + 1); // this isn't perfect, but we really should never get close to full
        size_t index = find_insert_pos(key);
        m_data.fill_or_overwrite(index, { std::forward<P>(key), std::forward<U>(val) });
        return index;
    }

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }
    inline bool max_load_factor_exceeded() const { return m_data.size() >= _max_load_factor; }

    inline void clear() { m_data.clear(); }

    size_t find_index(const K& key) const
    {
        size_t index = hash(key).val & _index_mask;
        for(; m_data.is_filled(index) && m_data[index].key != key; index = (index + 1) & _index_mask);
        index = (m_data.is_filled(index) && m_data[index].key == key) ? index : INVALID_SIZE_T;
        return index;
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

    size_t erase_index(size_t index)
    {
        if(index >= _capacity || index == INVALID_SIZE_T || !m_data.is_filled(index))
            return INVALID_SIZE_T;
        m_data.erase(index);
        // reorder the remaining items in the block
        for(size_t i = (index + 1) & _index_mask; m_data.is_filled(i); i = (i + 1) & _index_mask)
        {
            const size_t new_index = find_insert_pos(m_data[i].key);
            if(new_index != i)
            {
                m_data.fill(new_index, { std::move(m_data[i].key), std::move(m_data[i].value) });
                m_data.erase(i);
            }
        }
        return index;
    }

    inline size_t erase(const K& key)
    {
        return erase_index(find_index(key));
    }

    inline size_t begin_index() const { return m_data.begin_index(); }

    inline size_t next_index(size_t index) const { return m_data.next_index(index); }

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
    selftype& operator=(const stack_hashmap<K, V, _other_capacity, _hash_functor>& other)
    {
        assert(_max_load_factor >= other.size());
        clear();
        for(size_t index = other.begin_index(); index < other.capacity(); index = other.next_index(index))
        {
            insert(other.m_data[index].key, other.m_data[index].value);
        }
        return *this;
    }

    template <size_t _other_capacity>
    selftype& operator=(stack_hashmap<K, V, _other_capacity, _hash_functor>&& other)
    {
        assert(_max_load_factor >= other.size());
        clear();
        for(size_t index = other.begin_index(); index < other.capacity(); index = other.next_index(index))
        {
            insert(std::move(other.m_data[index].key), std::move(other.m_data[index].value));
        }
        other.clear();
        return *this;
    }

    // cstrs
    stack_hashmap() = default;
    stack_hashmap(const selftype & other) : m_data(other.m_data) {}
    stack_hashmap(selftype && other) : m_data(std::move(other.m_data)) {}
    template <size_t _other_capacity>
    stack_hashmap(const stack_hashmap<K, V, _other_capacity, _hash_functor>&other) { *this = other; }
    template <size_t _other_capacity>
    stack_hashmap(stack_hashmap<K, V, _other_capacity, _hash_functor> && other) { *this = std::move(other); }

    template <size_t _other_capacity>
    bool operator==(const stack_hashmap<K, V, _other_capacity, _hash_functor>& other) const
    {
        if(m_data.size() != other.m_data.size())
            return false;
        for(size_t index = begin_index(); index < _capacity; index = next_index(index))
        {
            const size_t i = other.find_index(m_data[index].key);
            if(i == INVALID_SIZE_T || other.m_data[i].value != m_data[index].value)
                return false;
        }
        return true;
    }
    
    inline kvpcref at_index(size_t i) const
    {
        assert(i < _capacity && m_data.is_filled(i));
        return { m_data[i].key, m_data[i].value };
    }

    inline kvpref at_index(size_t i)
    {
        assert(i < _capacity && m_data.is_filled(i));
        return { m_data[i].key, m_data[i].value };
    }

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
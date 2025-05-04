#pragma once
#include "stringutil.h"
#include <vector>

/*

This file contains the definitions for the string registries used in parsing

*/

namespace obn
{

constexpr size_t STRING_POOL_DEFAULT_CAPACITY_NUM_BITS = 14;
constexpr size_t STRING_POOL_DEFAULT_CAPACITY = 1ULL << STRING_POOL_DEFAULT_CAPACITY_NUM_BITS;
constexpr size_t STRING_POOL_STARTING_CAPACITY_NUM_BITS = 10;
constexpr size_t STRING_POOL_STARTING_CAPACITY = 1ULL << STRING_POOL_STARTING_CAPACITY_NUM_BITS;

constexpr size_t STRING_REGISTRY_DEFAULT_VIEW_CAPACITY_NUM_BITS = 12;
constexpr size_t STRING_REGISTRY_DEFAULT_VIEW_CAPACITY = 1ULL << STRING_REGISTRY_DEFAULT_VIEW_CAPACITY_NUM_BITS;

using string_pool_chartype = char;

/*

The string pool class is used for processing strings. It allows us to insert strings into a pool that is
pointer/address consistent(ie. a pointer into the internal data will always be valid no matter how large the pool gets).

*/

template<size_t _capacity>
class fixed_string_pool : public unmoveable, public uncopyable
{
private:
    size_t m_size;
    string_pool_chartype m_data[_capacity];

public:
    fixed_string_pool() : m_size(0) { mem_set_zero(m_data, _capacity); }

    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    const string_pool_chartype* add(const string_pool_chartype* str, size_t len)
    {
        if(m_size + len + 1 <= _capacity)
        {
            string_pool_chartype* retVal = &m_data[m_size];
            if(mem_copy(retVal, str, sizeof(string_pool_chartype) * len) == 0)
            {
                m_data[m_size + len] = 0;
                m_size += len + 1;
                return retVal;
            }
        }
        return nullptr;
    }
};

class string_pool : public uncopyable, public unmoveable
{
private:
    size_t m_size;
    size_t m_capacity;
    std::vector<data_bucket<string_pool_chartype>> m_mem_block;

public:
    string_pool(size_t mem_block_size = STRING_POOL_STARTING_CAPACITY);

    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }
    inline bool empty() const { return m_size == 0; }

    const string_pool_chartype* add(const string_pool_chartype* str, size_t len);
};

using view_index = size_t;
constexpr view_index view_index_invalid = INVALID_SIZE_T;

struct string_registry_view
{
    hash64_t hash;
    const string_pool_chartype* data;
    size_t len;

    inline bool operator<(const hash64_t& hash_val) const { return hash < hash_val; }
};

/*

The string registry holds metadata required to search through and utilise a string_pool

*/

template<size_t _pool_capacity, size_t _view_capacity>
class fixed_string_registry : public unmoveable, public uncopyable
{
private:
    fixed_string_pool<_pool_capacity> m_pool;
    size_t m_num_views;
    string_registry_view m_views[_view_capacity];

public:
    fixed_string_registry() : m_num_views(0) {}

    const string_pool_chartype* register_string(const string_pool_chartype* str, size_t len)
    {
        if(!str)
        {
            return nullptr;
        }

        const hash64_t hash = checksum64_string(str, len);
        size_t idx = binary_search_nearest(m_views, m_num_views, hash);

        for(; idx < m_num_views && m_views[idx].hash == hash; ++idx)
        {
            if(m_views[idx].len == len && string_ncmp(m_views[idx].data, str, len) == 0)
            {
                return m_views[idx].data;
            }
        }

        const string_pool_chartype* ptr = m_pool.add(str, len);
        if(!ptr)
        {
            return nullptr;
        }

        if(idx > m_num_views)
        {
            idx = m_num_views; // could have returned invalid
        }
        if(m_num_views > 0)
        {
            for(size_t i = m_num_views - 1; i >= idx && i <= m_num_views; --i)
            {
                m_views[i + 1] = m_views[i];
            }
        }
        m_views[idx] = { hash, ptr, len };
        m_num_views++;

        return ptr;
    }

    const string_pool_chartype* find_registered_string(const string_pool_chartype* str, size_t len) const
    {
        if(!str || m_num_views == 0)
        {
            return nullptr;
        }

        const hash64_t hash = checksum64_string(str, len);
        size_t idx = binary_search_nearest(m_views, m_num_views, hash);

        for(; idx < m_num_views && m_views[idx].hash == hash; ++idx)
        {
            if(m_views[idx].len == len && string_ncmp(m_views[idx].data, str, len) == 0)
            {
                return m_views[idx].data;
            }
        }

        return nullptr;
    }

    inline bool is_registered(const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) != nullptr;
    }

    inline size_t num_strings() const { return m_num_views; }
};

class string_registry : public uncopyable, public unmoveable
{
private:
    string_pool m_pool;
    std::vector<string_registry_view> m_views;

public:
    string_registry(size_t mem_block_size = STRING_POOL_STARTING_CAPACITY) : m_pool(mem_block_size) {}

    const string_pool_chartype* register_string(const string_pool_chartype* str, size_t len);

    const string_pool_chartype* find_registered_string(const string_pool_chartype* str, size_t len) const;

    inline bool is_registered(const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) != nullptr;
    }

    inline size_t num_strings() const { return m_views.size(); }
};

using fixed_string_registry_default = fixed_string_registry<STRING_POOL_DEFAULT_CAPACITY, STRING_REGISTRY_DEFAULT_VIEW_CAPACITY>;

}
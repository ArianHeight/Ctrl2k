#pragma once
#include "string_util.h"

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
class string_pool : public unmoveable, public uncopyable
{
private:
    size_t m_size;
    string_pool_chartype m_data[_capacity];

public:
    string_pool() : m_size(0) { mem_set_zero(m_data, _capacity); }

    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    const string_pool_chartype* add(const string_pool_chartype* str, size_t len)
    {
        if(m_size + len + 1 <= _capacity && str)
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

/*

The string registry holds metadata required to search through and utilise a string_pool

*/

using string_registry_id = uint32_t;
constexpr string_registry_id INVALID_STRING_REGISTRY_ID = UINT32_MAX;

struct string_registry_view
{
    hash64_t hash;
    string_registry_id id;
    uint32_t len;

    inline bool operator<(const hash64_t& hash_val) const { return hash < hash_val; }
};

class string_registry_base
{
protected:
    string_registry_base() = default;

public:
    virtual string_registry_id register_string(const string_pool_chartype* str, size_t len) = 0;
    virtual string_registry_id find_registered_string(const string_pool_chartype* str, size_t len) const = 0;

    inline bool is_registered(const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) != INVALID_STRING_REGISTRY_ID;
    }

    inline bool is_equal(const string_registry_id id, const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) == id;
    }

    virtual const string_pool_chartype* get_string(const string_registry_id id) const = 0;

    virtual size_t num_strings() const = 0;
};

template<size_t pool_capacity, size_t view_capacity>
class string_registry : public unmoveable, public uncopyable, public string_registry_base
{
private:
    string_pool<pool_capacity> m_pool;
    size_t m_num_views;
    string_registry_view m_views[view_capacity];
    const string_pool_chartype* m_id_map[view_capacity];

public:
    string_registry() : m_num_views(0) {}

    string_registry_id register_string(const string_pool_chartype* str, size_t len)
    {
        assert(len < UINT32_MAX);
        if(!str)
        {
            return INVALID_STRING_REGISTRY_ID;
        }

        const hash64_t hash = checksum64_string(str, len);
        size_t idx = binary_search_nearest(m_views, m_num_views, hash);

        for(; idx < m_num_views && m_views[idx].hash == hash; ++idx)
        {
            if(m_views[idx].len == len && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
            {
                return m_views[idx].id;
            }
        }

        const string_pool_chartype* ptr = m_pool.add(str, len);
        if(!ptr)
        {
            return INVALID_STRING_REGISTRY_ID;
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
        const string_registry_id new_id = (string_registry_id)m_num_views;
        m_views[idx] = { hash, new_id, (uint32_t)len };
        m_id_map[m_num_views] = ptr;
        m_num_views++;

        return new_id;
    }

    string_registry_id find_registered_string(const string_pool_chartype* str, size_t len) const
    {
        assert(len < UINT32_MAX);
        if(!str || m_num_views == 0)
        {
            return INVALID_STRING_REGISTRY_ID;
        }

        const hash64_t hash = checksum64_string(str, len);
        size_t idx = binary_search_nearest(m_views, m_num_views, hash);

        for(; idx < m_num_views && m_views[idx].hash == hash; ++idx)
        {
            if(m_views[idx].len == len && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
            {
                return m_views[idx].id;
            }
        }

        return INVALID_STRING_REGISTRY_ID;
    }

    const string_pool_chartype* get_string(const string_registry_id id) const override { return m_id_map[id]; }

    size_t num_strings() const override { return m_num_views; }
};

using string_registry_default = string_registry<STRING_POOL_DEFAULT_CAPACITY, STRING_REGISTRY_DEFAULT_VIEW_CAPACITY>;

}
#pragma once
#include "borrowed_string.h"
#include "simple_string.h"

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

    string_registry_view() : hash(INVALID_HASH64), id(INVALID_STRING_REGISTRY_ID), len(0) {}
    string_registry_view(hash64_t hash_val) : hash(hash_val), id(INVALID_STRING_REGISTRY_ID), len(0) {}
    string_registry_view(hash64_t hash_val, string_registry_id id_val, uint32_t len_val) : hash(hash_val), id(id_val), len(len_val) {}

    inline bool operator<(const string_registry_view& other) const { return hash < other.hash; }
};

class string_registry_base
{
protected:
    string_registry_base() = default;

public:
    virtual string_registry_id register_string(const string_pool_chartype* str, size_t len) = 0;
    virtual string_registry_id find_registered_string(const string_pool_chartype* str, size_t len) const = 0;

    inline string_registry_id register_string(const borrowed_string<string_pool_chartype>& str)
    {
        return register_string(str.c_str(), str.length());
    }

    template <template <typename U, size_t N> typename S, size_t _num, bool _dyn = false>
    inline string_registry_id register_string(const simple_string<S, string_pool_chartype, _num, _dyn>& str)
    {
        return register_string(str.c_str(), str.length());
    }

    inline string_registry_id find_registered_string(const borrowed_string<string_pool_chartype>& str) const
    {
        return find_registered_string(str.c_str(), str.length());
    }

    template <template <typename U, size_t N> typename S, size_t _num, bool _dyn = false>
    inline string_registry_id find_registered_string(const simple_string<S, string_pool_chartype, _num, _dyn>& str) const
    {
        return find_registered_string(str.c_str(), str.length());
    }

    inline bool is_registered(const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) != INVALID_STRING_REGISTRY_ID;
    }

    inline string_registry_id is_registered(const borrowed_string<string_pool_chartype>& str) const
    {
        return find_registered_string(str.c_str(), str.length()) != INVALID_STRING_REGISTRY_ID;
    }

    template <template <typename U, size_t N> typename S, size_t _num, bool _dyn = false>
    inline string_registry_id is_registered(const simple_string<S, string_pool_chartype, _num, _dyn>& str) const
    {
        return find_registered_string(str.c_str(), str.length()) != INVALID_STRING_REGISTRY_ID;
    }

    inline bool is_equal(const string_registry_id id, const string_pool_chartype* str, size_t len) const
    {
        return find_registered_string(str, len) == id;
    }

    inline bool is_equal(const string_registry_id id, const borrowed_string<string_pool_chartype>& str) const
    {
        return find_registered_string(str.c_str(), str.length()) == id;
    }

    template <template <typename U, size_t N> typename S, size_t _num, bool _dyn = false>
    inline bool is_equal(const string_registry_id id, const simple_string<S, string_pool_chartype, _num, _dyn>& str) const
    {
        return find_registered_string(str.c_str(), str.length()) == id;
    }

    virtual const string_pool_chartype* get_string(const string_registry_id id) const = 0;

    virtual size_t num_strings() const = 0;
};

template<size_t _pool_capacity, size_t _view_capacity>
requires (_view_capacity > 4 && is_exp_of_two(_view_capacity)) // needed to use bitmasking instead of modulus
class string_registry : public unmoveable, public uncopyable, public string_registry_base
{
    static constexpr size_t _max_view_load_factor = (size_t)(0.75f * _view_capacity);
    static constexpr size_t _view_index_mask = _view_capacity - 1;

private:
    string_pool<_pool_capacity> m_pool;
    size_t m_num_views;
    string_registry_view m_views[_view_capacity];
    const string_pool_chartype* m_id_map[_view_capacity];

public:
    string_registry() : m_num_views(0) {}

    string_registry_id register_string(const string_pool_chartype* str, size_t len)
    {
        assert(len < UINT32_MAX);
        assert(m_num_views < _max_view_load_factor);
        if(!str)
        {
            return INVALID_STRING_REGISTRY_ID;
        }

        const hash64_t hash = simple_hash(str, len);
        size_t idx = hash.val & _view_index_mask;
        for(; m_views[idx].hash != INVALID_HASH64; idx = (idx + 1) & _view_index_mask)
        {
            if(m_views[idx].len == len && m_views[idx].hash == hash && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
            {
                return m_views[idx].id;
            }
        }

        const string_pool_chartype* ptr = m_pool.add(str, len);
        if(!ptr)
        {
            return INVALID_STRING_REGISTRY_ID;
        }

        const string_registry_id new_id = (string_registry_id)m_num_views;
        m_views[idx] = { hash, new_id, (uint32_t)len };
        m_id_map[m_num_views] = ptr;
        ++m_num_views;

        return new_id;
    }

    string_registry_id find_registered_string(const string_pool_chartype* str, size_t len) const
    {
        assert(len < UINT32_MAX);
        if(!str || m_num_views == 0)
        {
            return INVALID_STRING_REGISTRY_ID;
        }

        const hash64_t hash = simple_hash(str, len);
        size_t idx = hash.val & _view_index_mask;
        for(; m_views[idx].hash != INVALID_HASH64; idx = (idx + 1) & _view_index_mask)
        {
            if(m_views[idx].len == len && m_views[idx].hash == hash && string_ncmp(m_id_map[m_views[idx].id], str, len) == 0)
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
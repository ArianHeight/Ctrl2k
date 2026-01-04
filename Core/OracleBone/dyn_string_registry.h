#pragma once
#include <vector>

#include "string_registry.h"

/*

This file contains the definitions for the dynamic version of string_registry

*/

namespace obn::dyn
{

// Refer to the normal, non-dynamic version of string_pool for more detailed comments
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

// Refer to the normal, non-dynamic version of string_registry for more detailed comments
class string_registry : public uncopyable, public unmoveable, public string_registry_base
{
private:
    string_pool m_pool;
    std::vector<string_registry_view> m_views;
    std::vector<const string_pool_chartype*> m_id_map;

public:
    string_registry(size_t mem_block_size = STRING_POOL_STARTING_CAPACITY) : m_pool(mem_block_size) {}

    string_registry_id register_string(const string_pool_chartype* str, size_t len) override;

    string_registry_id find_registered_string(const string_pool_chartype* str, size_t len) const override;

    const string_pool_chartype* get_string(const string_registry_id id) const override;

    size_t num_strings() const override;
};

}

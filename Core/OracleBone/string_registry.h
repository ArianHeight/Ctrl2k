#pragma once
#include "string_pool.h"

/*

This file contains the definitions for the string registries used in parsing

*/

namespace obn
{

using view_index = size_t;
constexpr view_index view_index_invalid = INVALID_SIZE_T;

struct string_registry_view
{
    hash64_t hash;
    string_pool_chartype* const data;
    size_t len;
};

template<size_t _pool_capacity, size_t _view_capacity>
class fixed_string_registry
{
private:
    fixed_string_pool<_pool_capacity> m_pool;
    size_t m_num_views;
    string_registry_view m_views[_view_capacity];

public:
    fixed_string_registry() : m_num_views(0) {}

    view_index register_string(const string_pool_chartype* str, size_t len)
    {
        if(!str)
        {
            return view_index_invalid;
        }

        hash64_t hash = checksum64_string(str, len);


    }
};

}
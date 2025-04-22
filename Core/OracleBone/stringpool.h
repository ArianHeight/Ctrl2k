#pragma once
#include "stringutil.h"

#include <vector>
#include <unordered_map>

/*

The string pool class is used for processing strings. It allows us to insert strings into a pool that is
pointer/address consistent(ie. a pointer into the internal data will always be valid no matter how large the pool gets).


*/

namespace obn
{

constexpr size_t STRING_POOL_DEFAULT_CAPACITY_NUM_BITS = 10;
constexpr size_t STRING_POOL_DEFAULT_CAPACITY = 1ULL << STRING_POOL_DEFAULT_CAPACITY_NUM_BITS;

class string_pool
{
public:
    using chartype = wchar_t;

    struct simple_view
    {
        size_t len;
        chartype* data;
    };

private:
    std::vector<data_bucket<chartype>> m_buckets;
    std::vector<simple_view> m_ptrCache;

public:
    
};

}
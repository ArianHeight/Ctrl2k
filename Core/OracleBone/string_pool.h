#pragma once
#include "stringutil.h"
#include <vector>

/*

The string pool class is used for processing strings. It allows us to insert strings into a pool that is
pointer/address consistent(ie. a pointer into the internal data will always be valid no matter how large the pool gets).


*/

namespace obn
{

constexpr size_t STRING_POOL_DEFAULT_CAPACITY_NUM_BITS = 10;
constexpr size_t STRING_POOL_DEFAULT_CAPACITY = 1ULL << STRING_POOL_DEFAULT_CAPACITY_NUM_BITS;

using string_pool_chartype = char;

class string_registry;

template<size_t _capacity>
class fixed_string_pool : public unmoveable
{
private:
    size_t m_size;
    string_pool_chartype m_data[_capacity];

public:
    fixed_string_pool() : m_size(0) {}

    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    const string_pool_chartype* add(const string_pool_chartype* str, size_t len)
    {
        if(m_size + len + 1 < _capacity)
        {
            string_pool_chartype* retVal = &m_data[m_size];
            if(mem_copy(retVal, str, sizeof(string_pool_chartype) * len))
            {
                m_data[m_size + len] = 0;
                m_size += len + 1;
                return retVal;
            }
        }
        return nullptr;
    }

    inline const string_pool_chartype operator[](size_t i) const { return m_data[i]; }
};

class string_pool : public uncopyable
{
public:
    friend string_registry;

private:
    std::vector<data_bucket<string_pool_chartype>> m_memBlock;

public:
    string_pool(size_t mem_block_size);
    string_pool(string_pool&& other);

    const string_pool_chartype* add(const string_pool_chartype* str, size_t len);
};

class string_registry : public uncopyable
{
private:
    struct view
    {
        string_pool_chartype* c_str;
        size_t len;
        hash64_t hash;
    };

    string_pool m_pool;
    std::vector<view> m_views; // sorted
    
public:
    string_registry(size_t mem_block_size);
    string_registry(string_registry&& other);

    const string_pool_chartype* insert(const string_pool_chartype* str, size_t len, hash64_t hash);
    const string_pool_chartype* find(const string_pool_chartype* str, size_t len, hash64_t hash);
    const string_pool_chartype* insert_or_find(const string_pool_chartype* str, size_t len, hash64_t hash);

    const string_pool_chartype* find_first(hash64_t hash);

    inline size_t num_strings() const { return m_views.size(); }
};

}
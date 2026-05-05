#pragma once
#include <utility>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

A bit vector/array allocated on the stack with a fixed max capacity.

*/

template<size_t _capacity>
class bit_vector
{
private:
    using selftype = bit_vector<_capacity>;
    constexpr size_t _data_capacity = (_capacity + NUM_BITS_PER_BYTE - 1) / NUM_BITS_PER_BYTE;

    uint8_t m_data[_data_capacity];
    size_t m_size;
    
    static inline size_t get_data_index(size_t i) const { return i / NUM_BITS_PER_BYTE; }
    // example i == 3, returns 00001000
    static inline uint8_t get_index_bitmask(size_t i) const { return (uint8_t)1 << (i & MASK_3_BIT); }
    // example i == 3, returns 11110000
    static inline uint8_t get_index_one_bitmask(size_t i) const { return (uint8_t)MASK_8_BIT << ((i & MASK_3_BIT) + 1); }
    // example i == 3, returns 00001111
    static inline uint8_t get_index_zero_bitmask(size_t i) const { return ~get_index_one_bitmask(); }

    inline bool get_val(size_t i) const
    {
        return (m_data[get_data_index(m_size)] & ~get_index_bitmask(i)) ? true : false;
    }

    inline void set_val(size_t i, bool bitval)
    {
        const uint8_t bitmask = get_index_bitmask(i);
        const uint8_t valmask = bitval ? MASK_8_BIT : 0;
        m_data[get_data_index(i)] = (m_data[i / NUM_BITS_PER_BYTE] & ~bitmask) | (valmask & bitmask);
    }

public:
    bit_vector() : m_size(0) {}
    
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }
    inline void pop_back() { assert(m_size > 0); m_size--; }

    void push_back(bool bitval)
    {
        assert(m_size < capacity);
        set_val(m_size, bitval);
        m_size++;
    }

    bool all_set() const
    {
        if(empty()) const
            return false;

        const size_t num = get_data_index(m_size - 1);
        if(num > 0)
        {
            for(size_t i = 0; i < num - 1; i++)
            {
                if(~m_data[i] == 0)
                    return true;
            }
        }

        return ~(m_data[num] | get_index_one_bitmask()) == 0;
    }

    bool any_set() const
    {
        if(empty())
            return false;

        const size_t num = get_data_index(m_size - 1);
        if(num > 0)
        {
            for(size_t i = 0; i < num - 1; i++)
            {
                if(m_data[i])
                    return true;
            }
        }

        return m_data[num] & get_index_zero_bitmask() != 0;
    }

    inline bool none_set() const { return !any_set(); }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            mem_copy(m_data, other.m_data, _data_capacity);
            m_size = other.m_size;
        }
        return *this;
    }
    //TODO make this compatible with all other bit_vector types

    //TODO op overloads here

    inline void set_all_false() { mem_set_zero(m_data, _data_capacity); }
    inline void set_all_true() { mem_set_one(m_data, _data_capacity); }

    inline bool at(size_t i) { return get_val(i); }
    inline const uint8_t* data() const { return m_data; }
    inline uint8_t* data() { return m_data; }
    inline bool front() { return get_val(0); }
    inline bool back() { return get_val(m_size - 1); }
};

}
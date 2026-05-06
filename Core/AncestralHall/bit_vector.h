#pragma once
#include <utility>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

Shared code for all bit vectors

*/

class a_bit_vector
{
protected:
    a_bit_vector() = default;

    static inline size_t get_data_index(size_t i) { return i / NUM_BITS_PER_BYTE; }
    // example i == 3, returns 00001000
    static inline uint8_t get_index_bitmask(size_t i) { return (uint8_t)1 << (i & MASK_3_BIT); }
    // example i == 3, returns 11110000
    static inline uint8_t get_index_one_bitmask(size_t i) { return (uint8_t)MASK_8_BIT << ((i & MASK_3_BIT) + 1); }
    // example i == 3, returns 00001111
    static inline uint8_t get_index_zero_bitmask(size_t i) { return ~get_index_one_bitmask(i); }
};

/*

A bit vector/array allocated on the stack with a fixed max capacity.

*/

template<size_t _capacity>
class bit_vector : public a_bit_vector
{
private:
    using selftype = bit_vector<_capacity>;
    static constexpr size_t _data_capacity = (_capacity + NUM_BITS_PER_BYTE - 1) / NUM_BITS_PER_BYTE;

    uint8_t m_data[_data_capacity];
    size_t m_size;

    inline bool get_val(size_t i) const
    {
        return (m_data[get_data_index(i)] & get_index_bitmask(i)) ? true : false;
    }

    inline void set_val(size_t i, bool bitval)
    {
        const uint8_t bitmask = get_index_bitmask(i);
        const uint8_t valmask = bitval ? MASK_8_BIT : 0;
        m_data[get_data_index(i)] = (m_data[get_data_index(i)] & ~bitmask) | (valmask & bitmask);
    }

public:
    bit_vector() : m_size(0) {}
    
    inline size_t size() const { return m_size; } // number of bits
    inline size_t capacity() const { return _capacity; } // max number of bits held
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }
    inline void pop_back() { assert(m_size > 0); m_size--; }

    void push_back(bool bitval)
    {
        assert(m_size < _capacity);
        set_val(m_size, bitval);
        m_size++;
    }

    void resize(size_t new_size, bool val = false)
    {
        if(new_size < m_size)
        {
            m_size = new_size;
        }
        else
        {
            new_size = (_capacity > new_size) ? new_size : _capacity;
            const size_t old_num = get_data_index(m_size - 1);
            const size_t num = get_data_index(new_size - 1);
            const uint8_t new_val = val ? MASK_8_BIT : 0;

            m_data[old_num] = (m_data[old_num] & get_index_zero_bitmask(m_size - 1)) | new_val;
            for(size_t i = old_num + 1; i < num; i++)
            {
                m_data[i] = new_val;
            }
            m_size = new_size;
        }
    }

    bool all_set() const
    {
        if(empty())
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

        return ~(m_data[num] | get_index_one_bitmask(m_size - 1)) == 0;
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

        return m_data[num] & get_index_zero_bitmask(m_size - 1) != 0;
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

    template<size_t _other_capacity>
    selftype& operator=(const bit_vector<_other_capacity>& other)
    {
        m_size = (other.m_size > _capacity) ? _capacity : other.m_size;
        mem_copy(m_data, other.m_data, (other._data_capacity > _data_capacity ? _data_capacity : other._data_capacity));
        return *this;
    }

    selftype operator~()
    {
        selftype retval;
        if(empty())
            return retval;

        retval.m_size = m_size;
        const size_t num = get_data_index(m_size - 1);
        for(size_t i = 0; i < num; i++)
        {
            retval.m_data[i] = ~m_data[i];
        }

        return retval;
    }

    template<size_t _other_capacity>
    selftype operator&(const bit_vector<_other_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.m_size) ? other.m_size : m_size;
        const size_t num = get_data_index(retval.m_size - 1);
        for(size_t i = 0; i < num; i++)
        {
            retval.m_data[i] = m_data[i] & other.m_data[i];
        }
        return retval;
    }

    template<size_t _other_capacity>
    selftype operator|(const bit_vector<_other_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.m_size) ? other.m_size : m_size;
        const size_t num = get_data_index(retval.m_size - 1);
        for(size_t i = 0; i < num; i++)
        {
            retval.m_data[i] = m_data[i] | other.m_data[i];
        }
        return retval;
    }

    template<size_t _other_capacity>
    selftype operator^(const bit_vector<_other_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.m_size) ? other.m_size : m_size;
        const size_t num = get_data_index(retval.m_size - 1);
        for(size_t i = 0; i < num; i++)
        {
            retval.m_data[i] = m_data[i] ^ other.m_data[i];
        }
        return retval;
    }

    inline void set_all_false() { mem_set_zero(m_data, _data_capacity); }
    inline void set_all_true() { mem_set_one(m_data, _data_capacity); }

    inline bool at(size_t i) const { return get_val(i); }
    inline bool operator[](size_t i) const { return get_val(i); }
    inline const uint8_t* data() const { return m_data; } // underlying data here is a byte
    inline uint8_t* data() { return m_data; } // underlying data here is a byte
    inline bool front() const { return get_val(0); }
    inline bool back() const { return get_val(m_size - 1); }
};

}
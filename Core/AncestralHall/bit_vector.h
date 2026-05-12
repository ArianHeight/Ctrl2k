#pragma once
#include <utility>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

Shared code for all bit vectors

*/

class abstract_bit_vector
{
protected:
    abstract_bit_vector() = default;

    // defining all constexprs in terms of the datatype being used to prevent resizing when used with operators.
    using datatype = uint32_t;
    static constexpr datatype DATA_TYPE_MAX = UINT32_MAX;
    static constexpr datatype DATA_INDEX_MASK = MASK_5_BIT;
    static constexpr datatype BITS_PER_DATA_TYPE = NUM_BITS_PER_BYTE * sizeof(datatype);

    static inline size_t get_data_index(size_t i) { return i / BITS_PER_DATA_TYPE; }
    // example i == 3, returns 00001000
    static inline datatype get_index_bitmask(size_t i) { return 1 << (i & DATA_INDEX_MASK); }
    // example i == 3, returns 11110000
    static inline datatype get_index_one_bitmask(size_t i) { return DATA_TYPE_MAX << ((i & DATA_INDEX_MASK) + 1); }
    // example i == 3, returns 00001111
    static inline datatype get_index_zero_bitmask(size_t i) { return ~get_index_one_bitmask(i); }
};

/*

A bit vector/array allocated on the stack with a fixed max capacity.

*/

template<size_t _data_capacity>
class bit_vector : public abstract_bit_vector
{
private:
    using selftype = bit_vector<_data_capacity>;
    static constexpr size_t _capacity = _data_capacity * BITS_PER_DATA_TYPE;

    datatype m_data[_data_capacity];
    size_t m_size;

    inline bool get_val(size_t i) const
    {
        return (m_data[get_data_index(i)] & get_index_bitmask(i)) ? true : false;
    }

    inline void set_val(size_t i, bool bitval)
    {
        const datatype bitmask = get_index_bitmask(i);
        const datatype valmask = bitval ? DATA_TYPE_MAX : 0;
        m_data[get_data_index(i)] = (m_data[get_data_index(i)] & ~bitmask) | (valmask & bitmask);
    }

public:
    inline size_t size() const { return m_size; } // number of bits
    inline size_t capacity() const { return _capacity; } // max number of bits held
    inline bool empty() const { return m_size == 0; }

    inline constexpr size_t data_capacity() const { return _data_capacity; }

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
        index_assert(new_size, _capacity);
        if(new_size <= m_size)
        {
            m_size = new_size;
        }
        else
        {
            new_size = (_capacity > new_size) ? new_size : _capacity;
            const datatype new_val = val ? DATA_TYPE_MAX : 0;

            size_t i = 0;
            if(m_size > 0)
            {
                const size_t old_last_index = get_data_index(m_size - 1);
                m_data[old_last_index] = (m_data[old_last_index] & get_index_zero_bitmask(m_size - 1)) | (new_val & get_index_one_bitmask(m_size - 1));
                i = old_last_index + 1;
            }
            
            const size_t data_size = get_data_index(new_size - 1) + 1;
            for(; i < data_size; i++)
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

        const size_t last_index = get_data_index(m_size - 1);
        for(size_t i = 0; i < last_index; i++)
        {
            if(~m_data[i] != 0)
                return false;
        }

        return ~(m_data[last_index] | get_index_one_bitmask(m_size - 1)) == 0;
    }

    bool any_set() const
    {
        if(empty())
            return false;

        const size_t last_index = get_data_index(m_size - 1);
        for(size_t i = 0; i < last_index; i++)
        {
            if(m_data[i])
                return true;
        }

        return (m_data[last_index] & get_index_zero_bitmask(m_size - 1)) != 0;
    }

    inline bool none_set() const { return !any_set(); }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            mem_copy(m_data, other.data(), sizeof(datatype) * _data_capacity);
            m_size = other.size();
        }
        return *this;
    }

    template<size_t _other_data_capacity>
    selftype& operator=(const bit_vector<_other_data_capacity>& other)
    {
        assert(other.size() <= _capacity);
        m_size = other.size();
        mem_copy(m_data, other.data(), sizeof(datatype) * (other.data_capacity() > _data_capacity ? _data_capacity : other.data_capacity()));
        return *this;
    }

    // cstrs
    bit_vector() : m_size(0) {}
    bit_vector(const selftype& other) : m_size(0) { *this = other; }
    template<size_t _other_data_capacity>
    bit_vector(const bit_vector<_other_data_capacity>& other) : m_size(0) { *this = other; }

    selftype operator~()
    {
        selftype retval;
        if(empty())
            return retval;

        retval.m_size = m_size;
        const size_t data_size = get_data_index(m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            retval.m_data[i] = ~m_data[i];
        }

        return retval;
    }

    // TODO how do we want to deal with doing these operations on different sized bit vectors?
    // Currently it just only applies the operation to the min set of the two.
    template<size_t _other_data_capacity>
    selftype operator&(const bit_vector<_other_data_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(retval.m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            retval.m_data[i] = m_data[i] & other.data()[i];
        }
        return retval;
    }

    template<size_t _other_data_capacity>
    selftype operator|(const bit_vector<_other_data_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(retval.m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            retval.m_data[i] = m_data[i] | other.data()[i];
        }
        return retval;
    }

    template<size_t _other_data_capacity>
    selftype operator^(const bit_vector<_other_data_capacity>& other) const
    {
        selftype retval;
        if(empty() || other.empty())
            return retval;

        retval.m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(retval.m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            retval.m_data[i] = m_data[i] ^ other.data()[i];
        }
        return retval;
    }

    template<size_t _other_data_capacity>
    selftype& operator&=(const bit_vector<_other_data_capacity>& other)
    {
        if(empty() || other.empty())
            return *this;

        m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            m_data[i] &= other.data()[i];
        }
        return *this;
    }

    template<size_t _other_data_capacity>
    selftype& operator|=(const bit_vector<_other_data_capacity>& other)
    {
        if(empty() || other.empty())
            return *this;

        m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            m_data[i] |= other.data()[i];
        }
        return *this;
    }

    template<size_t _other_data_capacity>
    selftype& operator^=(const bit_vector<_other_data_capacity>& other)
    {
        if(empty() || other.empty())
            return *this;

        m_size = (m_size > other.size()) ? other.size() : m_size;
        const size_t data_size = get_data_index(m_size - 1) + 1;
        for(size_t i = 0; i < data_size; i++)
        {
            m_data[i] ^= other.data()[i];
        }
        return *this;
    }

    template<size_t _other_data_capacity>
    bool operator==(const bit_vector<_other_data_capacity>& other) const
    {
        if(m_size != other.size())
            return false;
        else if(empty())
            return true;

        const size_t last_index = get_data_index(m_size - 1);
        if(last_index > 0)
        {
            for(size_t i = 0; i < last_index - 1; i++)
            {
                if((m_data[i] ^ other.data()[i]) != 0)
                    return false;
            }
        }

        const datatype bitmask = get_index_zero_bitmask(m_size - 1);
        return ((m_data[last_index] & bitmask) ^ (other.data()[last_index] & bitmask)) == 0;
    }

    inline void set_all_false() { mem_set_zero(m_data, sizeof(datatype) * ((get_data_index(m_size - 1) + 1) < _data_capacity ? (get_data_index(m_size - 1) + 1) : _data_capacity)); }
    inline void set_all_true() { mem_set_one(m_data, sizeof(datatype) * ((get_data_index(m_size - 1) + 1) < _data_capacity ? (get_data_index(m_size - 1) + 1) : _data_capacity)); }
    inline void set_bit(size_t i, bool val) { set_val(i, val); }
    inline bool get_bit(size_t i) const { return get_val(i); }

    inline bool at(size_t i) const { return get_val(i); }
    inline bool operator[](size_t i) const { return get_val(i); }
    inline const datatype* data() const { return m_data; } // underlying data here is a uint
    inline datatype* data() { return m_data; } // underlying data here is a uint
    inline bool front() const { return get_val(0); }
    inline bool back() const { return get_val(m_size - 1); }
};

}
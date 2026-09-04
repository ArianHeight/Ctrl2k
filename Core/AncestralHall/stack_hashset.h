#pragma once
#include "stack_vector.h"
#include "stack_bitvector.h"

namespace ahl
{

/*

This is a set implementation that uses hashing instead of sorting.
This uses stack_vector as the underlying data container, so it has a max capacity.

Because this uses hashing, collisions should be avoided if possible by having a capacity around 2x what is needed.

*/

template<typename T, size_t _capacity, class _hash_functor>
requires (_capacity > 0 && is_exp_of_two(_capacity)) // needed to use bitmasking instead of modulus
class stack_hashset
{
    template<typename T2, size_t _capacity2, class _hash_functor2>
    requires (_capacity > 0 && is_exp_of_two(_capacity))
    friend class stack_hashset;

private:
    using selftype = stack_hashset<T, _capacity, _hash_functor>;

    static constexpr size_t _max_load_factor = (size_t)(0.75f * _capacity);

    stack_vector<T, _capacity> m_data; // TODO we need a sparse vector impl
    stack_bitvector<_capacity> m_filled;
    _hash_functor hash;

    inline size_t find_insert_pos(const T& val) const
    {
        size_t index = hash(val);
    }

public:
    inline size_t size() const { return m_data.size(); }
    inline size_t capacity() const { return _capacity; }
    inline bool empty() const { return m_data.empty(); }
    inline bool max_load_factor_exceeded() const { return m_data.size() >= _max_load_factor; }

    inline void clear() { m_data.clear(); }
};

}
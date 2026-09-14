#pragma once
#include "stack_maskedarray.h"

namespace ahl
{

/*

This is a map implementation that uses hashing instead of sorting.
This uses stack_maskedarray as the underlying data container, so it has a max capacity.

Because this uses hashing, collisions should be avoided if possible by having a capacity around 2x what is needed.

*/

template <typename K, typename V, size_t _capacity, class _hash_functor = _generic_hash_functor<K>>
requires (_capacity > 0 && is_exp_of_two(_capacity)) // needed to use bitmasking instead of modulus
class stack_hashmap
{

};

}
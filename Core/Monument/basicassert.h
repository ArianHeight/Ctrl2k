#pragma once

#include <cassert>

#define range_assert(index, min, size) assert(index >= min && index < size)

#define index_assert(index, size) range_assert(index, 0, size)

#define internal_range_assert(start, internal_start, internal_len, len)\
assert( internal_start >= start && internal_start + internal_len <= start + len )

#define internal_full_range_assert(internal_start, internal_len, len) internal_range_assert(0, internal_start, internal_len, len)
#define full_internal_full_range_assert(internal_len, len) internal_range_assert(0, 0, internal_len, len)

#define ptr_assert(ptr) assert(ptr)

#define compile_assert(cond) static_assert(cond)

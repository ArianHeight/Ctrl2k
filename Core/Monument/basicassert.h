#pragma once

#include <cassert>

#define range_assert(index, min, size) assert(index >= min && index < size)

#define index_assert(index, size) range_assert(index, 0, size)

#define ptr_assert(ptr) assert(ptr)

#define compile_assert(cond) static_assert(cond)

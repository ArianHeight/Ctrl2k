#pragma once

#include <cassert>

#define assertIndex(index, size) assert(index >= 0 && index < size)

#define compile_assert(cond) static_assert(cond)
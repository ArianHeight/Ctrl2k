#pragma once
#include <string>

inline void mem_set_zero(void* dest, size_t count) { std::memset(dest, 0, count); }
inline void mem_set_one(void* dest, size_t count) { std::memset(dest, UINT8_MAX, count); }
inline errno_t mem_copy(void* dst, const void* src, size_t count) { return memcpy_s(dst, count, src, count); }
#pragma once
#include <string.h>
#include "Core/Monument/Monument.h"

namespace obn
{

template <typename chartype>
inline errno_t string_ncopy(chartype* dest, const chartype* src, size_t buf_size)
{
    if constexpr(sizeof(chartype) == sizeof(char))
        return strncpy_s(dest, buf_size, src, buf_size - 1);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsncpy_s(dest, buf_size, src, buf_size - 1);
    else
        static_assert(false);
}

template <typename chartype>
inline size_t string_nlen(const chartype* str, size_t buf_size)
{
    if constexpr(sizeof(chartype) == sizeof(char))
        return strnlen_s(str, buf_size);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsnlen_s(str, buf_size);
    else
        static_assert(false);
}

// could be dangerous
template <typename chartype>
inline size_t string_len(const chartype* str)
{
    return string_nlen(str, SIZE_MAX);
}

template <typename chartype>
inline int string_ncmp(const chartype* lhs, const chartype* rhs, size_t buf_size)
{
    if constexpr(sizeof(chartype) == sizeof(char))
        return strncmp(lhs, rhs, buf_size);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsncmp(lhs, rhs, buf_size);
    else
        static_assert(false);
}

}
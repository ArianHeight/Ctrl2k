#pragma once
#include <string.h>
#include "Core/Monument/Monument.h"

namespace obn
{

/*

Copies string data from src to dest(which has a capacity/buffer size of buf_size).
Truncates the string properly if src length is longer than buf_size - 1.
dest will always be a valid c string.
returns 0 if successful.
returns non zero if truncation occurred.

*/
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

/*

returns the smaller of max_len or the length of the given string(length excluding the trailing 0)

*/
template <typename chartype>
inline size_t string_nlen(const chartype* str, size_t max_len)
{
    if constexpr(sizeof(chartype) == sizeof(char))
        return strnlen_s(str, max_len);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsnlen_s(str, max_len);
    else
        static_assert(false);
}

// could be dangerous, consider using string_nlen instead
template <typename chartype>
inline size_t string_len(const chartype* str)
{
    return string_nlen(str, SIZE_MAX - 1);
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
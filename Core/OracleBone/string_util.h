#pragma once
#include <string.h>
#include <cctype>
#include <cwctype>
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
inline errno_t string_ncopy(chartype* dest, const chartype* src, const size_t buf_size)
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
inline size_t string_nlen(const chartype* str, const size_t max_len)
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

// returns 0 if the strings match
template <typename chartype>
inline int string_ncmp(const chartype* lhs, const chartype* rhs, const size_t buf_size)
{
    if constexpr(sizeof(chartype) == sizeof(char))
        return strncmp(lhs, rhs, buf_size);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsncmp(lhs, rhs, buf_size);
    else
        static_assert(false);
}

// the n in the name indicates this requires a buffer size count, it does not indicate not equal
template <typename chartype>
inline bool string_neq(const chartype* lhs, const chartype* rhs, const size_t buf_size)
{
    return string_ncmp(lhs, rhs, buf_size) == 0;
}

template <typename chartype>
const chartype* string_nchr(const chartype* str, const size_t len, const chartype chr)
{
    const chartype* const end = str + len;
    for(; str < end; ++str)
    {
        if(*str == chr)
            return str;
    }
    return nullptr;
}

template <typename chartype>
const chartype* string_nrchr(const chartype* str, const size_t len, const chartype chr)
{
    for(const chartype* end = str + len; end >= str; --end)
    {
        if(*end == chr)
            return end;
    }
    return nullptr;
}

template <typename chartype>
size_t string_nfind(const chartype* str, const size_t str_len, const chartype* substr, const size_t substr_len, const size_t start = 0)
{
    // why does std::string return start pos for 0 length substrings?
    if(str_len < substr_len || start > str_len - substr_len || substr_len == 0)
        return INVALID_SIZE_T;

    const chartype* end = str + (str_len - substr_len + 1);
    for(const chartype* c = str; c < end; ++c)
    {
        c = string_nchr(c, end - c, *substr);

        if(!c)
            return INVALID_SIZE_T;
        else if(string_neq(c, substr, substr_len))
            return c - str;
    }
    return INVALID_SIZE_T;
}

template <typename chartype>
inline size_t string_nfind(const chartype* str, const size_t str_len, const chartype chr, const size_t start = 0)
{
    if(start >= str_len)
        return INVALID_SIZE_T;
    return string_nchr(str + start, str_len, chr) - str;
}

template <typename chartype>
size_t string_nrfind(const chartype* str, const size_t str_len, const chartype* substr, const size_t substr_len, const size_t start)
{
    // why does std::string return start pos for 0 length substrings?
    if(str_len < substr_len || substr_len == 0)
        return INVALID_SIZE_T;
    
    for(const chartype* end = str + ((str_len - substr_len > start) ? start : (str_len - substr_len)); end >= str; --end)
    {
        end = string_nrchr(str, end - str, *substr);

        if(!end)
            return INVALID_SIZE_T;
        else if(string_neq(end, substr, substr_len))
            return end - str;
    }
    return INVALID_SIZE_T;
}
template <typename chartype>
inline size_t string_nrfind(const chartype* str, const size_t str_len, const chartype* substr, const size_t substr_len)
{
    return string_nrfind(str, str_len, substr, substr_len, str_len);
}

template <typename chartype>
inline size_t string_nrfind(const chartype* str, const size_t str_len, const chartype chr, const size_t start)
{
    return string_nrchr(str, str_len > start ? start : str_len, chr) - str;
}
template <typename chartype>
inline size_t string_nrfind(const chartype* str, const size_t str_len, const chartype chr)
{
    return string_nrchr(str, str_len, chr) - str;
}

#define DEFINE_CHAR_IS_TEMPLATE(charfuncname, wcharfuncname)                \
template <typename chartype>                                                \
inline bool char_##charfuncname(const chartype c)                           \
{                                                                           \
    if constexpr(sizeof(chartype) == sizeof(char))                          \
        return std::##charfuncname(static_cast<unsigned char>(c)) != 0;     \
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))                  \
        return std::##wcharfuncname(c) != 0;                                \
    else                                                                    \
        static_assert(false);                                               \
}

DEFINE_CHAR_IS_TEMPLATE(isalpha, iswalpha)
DEFINE_CHAR_IS_TEMPLATE(isalnum, iswalnum)
DEFINE_CHAR_IS_TEMPLATE(islower, iswlower)
DEFINE_CHAR_IS_TEMPLATE(isupper, iswupper)
DEFINE_CHAR_IS_TEMPLATE(isdigit, iswdigit)
DEFINE_CHAR_IS_TEMPLATE(isxdigit, iswxdigit)
DEFINE_CHAR_IS_TEMPLATE(ispunct, iswpunct)
DEFINE_CHAR_IS_TEMPLATE(isgraph, iswgraph)
DEFINE_CHAR_IS_TEMPLATE(isblank, iswblank)
DEFINE_CHAR_IS_TEMPLATE(isspace, iswspace)
DEFINE_CHAR_IS_TEMPLATE(isprint, iswprint)
DEFINE_CHAR_IS_TEMPLATE(iscntrl, iswcntrl)

}
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
inline errno_t string_ncopy(char* dest, const char* src, const size_t buf_size)
{
    return strncpy_s(dest, buf_size, src, buf_size - 1);
}

inline errno_t string_ncopy(wchar_t* dest, const wchar_t* src, const size_t buf_size)
{
    return wcsncpy_s(dest, buf_size, src, buf_size - 1);
}

/*

returns the smaller of max_len or the length of the given string(length excluding the trailing 0)

*/
inline size_t string_nlen(const char* str, const size_t max_len)
{
    return strnlen_s(str, max_len);
}

inline size_t string_nlen(const wchar_t* str, const size_t max_len)
{
    return wcsnlen_s(str, max_len);
}

// could be dangerous, consider using string_nlen instead
inline size_t string_len(const char* str) { return string_nlen(str, SIZE_MAX - 1); }
inline size_t string_len(const wchar_t* str) { return string_nlen(str, SIZE_MAX - 1); }

// returns 0 if the strings match
inline int string_ncmp(const char* lhs, const char* rhs, const size_t buf_size)
{
    return strncmp(lhs, rhs, buf_size);
}

inline int string_ncmp(const wchar_t* lhs, const wchar_t* rhs, const size_t buf_size)
{
    return wcsncmp(lhs, rhs, buf_size);
}

// the n in the name indicates this requires a buffer size count, it does not indicate not equal
inline bool string_neq(const char* lhs, const char* rhs, const size_t buf_size) { return string_ncmp(lhs, rhs, buf_size) == 0; }
inline bool string_neq(const wchar_t* lhs, const wchar_t* rhs, const size_t buf_size) { return string_ncmp(lhs, rhs, buf_size) == 0; }

template <typename T>
requires (TYPE_CHAR<T>)
const T* string_nchr(const T* str, const size_t len, const T chr)
{
    const T* const end = str + len;
    for(; str < end; ++str)
    {
        if(*str == chr)
            return str;
    }
    return nullptr;
}

template <typename T>
requires (TYPE_CHAR<T>)
const T* string_nrchr(const T* str, const size_t len, const T chr)
{
    for(const T* end = str + len; end >= str; --end)
    {
        if(*end == chr)
            return end;
    }
    return nullptr;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nfind(const T* str, const size_t str_len, const T* substr, const size_t substr_len, const size_t start = 0)
{
    // why does std::string return start pos for 0 length substrings?
    if(str_len < substr_len || start > str_len - substr_len || substr_len == 0)
        return INVALID_SIZE_T;

    const T* end = str + (str_len - substr_len + 1);
    for(const T* c = str; c < end; ++c)
    {
        c = string_nchr(c, end - c, *substr);

        if(!c)
            return INVALID_SIZE_T;
        else if(string_neq(c, substr, substr_len))
            return c - str;
    }
    return INVALID_SIZE_T;
}

template <typename T>
requires (TYPE_CHAR<T>)
inline size_t string_nfind(const T* str, const size_t str_len, const T chr, const size_t start = 0)
{
    if(start >= str_len)
        return INVALID_SIZE_T;
    return string_nchr(str + start, str_len, chr) - str;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nrfind(const T* str, const size_t str_len, const T* substr, const size_t substr_len, const size_t start = INVALID_SIZE_T)
{
    // why does std::string return start pos for 0 length substrings?
    if(str_len < substr_len || substr_len == 0)
        return INVALID_SIZE_T;
    
    for(const T* end = str + ((str_len - substr_len > start) ? start : (str_len - substr_len)); end >= str; --end)
    {
        end = string_nrchr(str, end - str, *substr);

        if(!end)
            return INVALID_SIZE_T;
        else if(string_neq(end, substr, substr_len))
            return end - str;
    }
    return INVALID_SIZE_T;
}

template <typename T>
requires (TYPE_CHAR<T>)
inline size_t string_nrfind(const T* str, const size_t str_len, const T chr, const size_t start = INVALID_SIZE_T)
{
    return string_nrchr(str, str_len > start ? start : str_len, chr) - str;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nfind_first_of(const T* str, const size_t str_len, const T* charset, const size_t charset_len, const size_t start = 0)
{
    if(charset_len == 0 || start >= str_len)
        return INVALID_SIZE_T;

    const T* end = str + str_len;
    for(const T* c = str + start; c < end; ++c)
    {
        if(string_nchr(charset, charset_len, *c))
            return c - str;
    }
    return INVALID_SIZE_T;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nfind_first_not_of(const T* str, const size_t str_len, const T* charset, const size_t charset_len, const size_t start = 0)
{
    if(charset_len == 0 || start >= str_len)
        return INVALID_SIZE_T;

    const T* end = str + str_len;
    for(const T* c = str + start; c < end; ++c)
    {
        if(!string_nchr(charset, charset_len, *c))
            return c - str;
    }
    return INVALID_SIZE_T;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nfind_last_of(const T* str, const size_t str_len, const T* charset, const size_t charset_len, const size_t start = INVALID_SIZE_T)
{
    if(charset_len == 0)
        return INVALID_SIZE_T;
    
    for(const T* end = str + ((str_len - 1 > start) ? start : (str_len - 1)); end >= str; --end)
    {
        if(string_nchr(charset, charset_len, *end))
            return end - str;
    }
    return INVALID_SIZE_T;
}

template <typename T>
requires (TYPE_CHAR<T>)
size_t string_nfind_last_not_of(const T* str, const size_t str_len, const T* charset, const size_t charset_len, const size_t start = INVALID_SIZE_T)
{
    if(charset_len == 0)
        return INVALID_SIZE_T;

    for(const T* end = str + ((str_len - 1 > start) ? start : (str_len - 1)); end >= str; --end)
    {
        if(!string_nchr(charset, charset_len, *end))
            return end - str;
    }
    return INVALID_SIZE_T;
}

#define DEFINE_CHAR_IS_TEMPLATE(charfuncname, wcharfuncname)                \
template <typename T>                                                       \
inline bool char_##charfuncname(const T c)                                  \
requires (TYPE_CHAR<T>)                                                     \
{                                                                           \
    if constexpr(sizeof(T) == sizeof(char))                                 \
        return std::##charfuncname(static_cast<unsigned char>(c)) != 0;     \
    else if constexpr(sizeof(T) == sizeof(wchar_t))                         \
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
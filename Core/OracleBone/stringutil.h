#pragma once
#include <string.h>
#include <type_traits>
#include "Core/Monument/basictypes.h"

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
        return strncmp_s(lhs, rhs, buf_size);
    else if constexpr(sizeof(chartype) == sizeof(wchar_t))
        return wcsncmp_s(lhs, rhs, buf_size);
    else
        static_assert(false);
}

template <template <typename U, size_t N> typename datatype, typename chartype, size_t num, bool dyn = false>
struct simple_string
{
private:
    using fulltype = datatype<chartype, num>;
    using selftype = simple_string<datatype, chartype, num, dyn>;

    fulltype internal_data;

public:
    inline bool has_error() const { return internal_data.error; }
    inline const chartype* c_str() const { return internal_data.buf; }
    inline const chartype* data() const { return internal_data.buf; }
    inline size_t length() const { return internal_data.len; }
    inline size_t size() const { return internal_data.len; }
    inline size_t capacity() const{ return internal_data.capacity; }
    inline size_t max_length() const { return internal_data.max_len; }

    inline void shrink_to_fit() const { if constexpr(dyn) internal_data.shrink_capacity(); }

    void copy(const chartype* str)
    {
        if constexpr(dyn)
        {
            internal_data.len = string_len(str);
            internal_data.increase_capacity(internal_data.len + 1);
        }

        internal_data.error = string_ncopy(internal_data.buf, str, internal_data.capacity) != 0;

        if constexpr(!dyn)
            internal_data.len = string_nlen(internal_data.buf, internal_data.capacity);
    }

    template <template <typename U2, size_t N2> typename datatype2, typename chartype, size_t num2, bool dyn2>
    void copy(const simple_string<datatype2, chartype, num2, dyn2>& other)
    {
        if constexpr(dyn)
            internal_data.increase_capacity(other.length() + 1);

        string_ncopy(internal_data.buf, other.c_str(), internal_data.capacity);
        internal_data.error = other.has_error();
        internal_data.len = other.length();
    }

    simple_string() {}
    simple_string(const chartype* str) { copy(str); }

    inline selftype& operator=(const chartype* str) { copy(str); return *this; }

    template <template <typename U2, size_t N2> typename datatype2, typename chartype, size_t num2, bool dyn2>
    inline selftype& operator=(const simple_string<datatype2, chartype, num2, dyn2>& other) { copy(other); return *this; }
};

}
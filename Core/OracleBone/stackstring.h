#pragma once
#include "simplestring.h"

namespace obn
{

/*

DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface

This is a form of compressed string where we use a total of 2^2, 2^3, ..., 2^7 chars for storing everything.
Allocated on the stack.

*/
template <typename chartype, size_t exp>
requires (exp < 8 && exp > 1)
struct small_string_data
{
    static constexpr size_t capacity = (1 << exp) - 1;
    static constexpr size_t max_len = capacity - 1;

    bool error : 1;
    uint8_t len : exp;
    chartype buf[capacity];

    small_string_data() : error(false), len(0) { buf[0] = 0; }
};

/*

DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface

This is a standard fixed string allocated on the stack.

*/
template <typename chartype, size_t buf_size>
struct fixed_string_data
{
    static constexpr size_t capacity = buf_size;
    static constexpr size_t max_len = capacity - 1;

    size_t len;
    bool error;
    chartype buf[capacity];

    fixed_string_data() : len(0), error(false) { buf[0] = 0; }
};

typedef simple_string<small_string_data, char, 3> small_string8;
typedef simple_string<small_string_data, char, 4> small_string16;
typedef simple_string<small_string_data, char, 5> small_string32;
typedef simple_string<small_string_data, char, 6> small_string64;
typedef simple_string<small_string_data, char, 7> small_string128;

typedef simple_string<small_string_data, wchar_t, 3> wsmall_string8;
typedef simple_string<small_string_data, wchar_t, 6> wsmall_string64;
typedef simple_string<small_string_data, wchar_t, 7> wsmall_string128;

template <size_t buf_size>
using fixed_string = simple_string<fixed_string_data, char, buf_size>;
template <size_t buf_size>
using wfixed_string = simple_string<fixed_string_data, wchar_t, buf_size>;

}
#pragma once
#include "stringutil.h"

namespace obn
{

// DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface
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

// DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface
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

template <size_t size>
using fixed_string = simple_string<fixed_string_data, char, size>;

}
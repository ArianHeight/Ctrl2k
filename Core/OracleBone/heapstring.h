#pragma once
#include "stringutil.h"

namespace obn
{

// DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface
template <typename chartype, size_t initial_size>
struct dynamic_string_data
{
    static constexpr size_t max_len = SIZE_MAX - 1;
    inline static chartype zero_val = 0; // please do not touch this

    size_t capacity;
    size_t len;
    bool error;
    chartype* buf;

    dynamic_string_data() : capacity(initial_size), len(0), error(false), buf(&zero_val)
    {
        if constexpr(initial_size != 0)
        {
            buf = new chartype[initial_size];
            buf[0] = 0;
        }
    }

    ~dynamic_string_data()
    {
        if(buf && buf != &zero_val)
            delete[] buf;
    }

    void change_capacity(size_t target_capacity)
    {
        if(buf && buf != &zero_val)
            delete[] buf;
        if(target_capacity > 0)
        {
            buf = new chartype[target_capacity];
            buf[0] = 0;
            capacity = target_capacity;
        }
        else
        {
            buf = &zero_val;
            capacity = 1;
        }
    }

    void increase_capacity(size_t target_capacity)
    {
        if(target_capacity <= capacity)
            return;
        change_capacity(target_capacity);
    }

    inline void shrink_capacity()
    {
        change_capacity(len + 1);
    }
};

typedef simple_string<dynamic_string_data, char, 0, true> dynamic_string0;
typedef simple_string<dynamic_string_data, char, 32, true> dynamic_string;

}
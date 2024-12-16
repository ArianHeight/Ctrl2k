#pragma once
#include "simplestring.h"

namespace obn
{

/*

DO NOT USE DIRECTLY, use the related typedefs with a simple_string interface

This is a dynamic string with no local buffer(except when capacity is 1 or lower).
When the string length is 0 and we shrink the capacity, the internal buffer will point to a static 0 value char.
Otherwise, this string always dynamically allocates memory from the heap to store string data.
This has ownership over its own data.

*/
template <typename chartype, size_t initial_size>
struct dynamic_string_data
{
    static constexpr size_t max_len = SIZE_MAX - 1;
    inline static chartype zero_val = 0; // please do not touch this

    size_t capacity;
    size_t len;
    bool error;
    chartype* buf;

    inline bool owns_memory() { return buf && buf != &zero_val; }

    // This is dangerous, as buf and len are left as invalid values
    // only designed to be called from simple_string and itself
    inline void delete_memory() { if(owns_memory()) delete[] buf; }

    dynamic_string_data() : capacity(initial_size), len(0), error(false), buf(&zero_val)
    {
        if constexpr(initial_size != 0)
        {
            buf = new chartype[initial_size];
            buf[0] = 0;
        }
    }

    ~dynamic_string_data() { delete_memory(); }

    // This is dangerous, as we do not free the buffer
    // only designed to be called from simple_string and itself
    void reset_values()
    {
        buf = &zero_val;
        capacity = 1;
        len = 0;
        error = false;
    }

private:
    // This is dangerous, as it does not always update len
    void change_capacity_erase_data(size_t target_capacity)
    {
        delete_memory();
        if(target_capacity > 1)
        {
            buf = new chartype[target_capacity];
            buf[0] = 0;
            capacity = target_capacity;
        }
        else
        {
            reset_values();
        }
    }

public:
    // can return false if shrinking causes truncation
    void change_capacity(size_t target_capacity)
    {
        if(target_capacity == capacity)
            return;
        if(target_capacity > 1)
        {
            chartype* new_buf = new chartype[target_capacity];
            error = string_ncopy(new_buf, buf, target_capacity) != 0;
            delete_memory();
            buf = new_buf;
            len = target_capacity < capacity ? target_capacity - 1 : len;
            capacity = target_capacity;
        }
        else
        {
            delete_memory();
            reset_values();
        }
    }

    void increase_capacity_erase_data(size_t target_capacity)
    {
        if(target_capacity <= capacity)
            return;
        change_capacity_erase_data(target_capacity);
    }

    void shrink_to_fit()
    {
        if(len + 1 >= capacity)
            return;
        change_capacity(len + 1);
    }

};

typedef simple_string<dynamic_string_data, char, 0, true> dynamic_string0;
typedef simple_string<dynamic_string_data, char, 32, true> dynamic_string;
typedef simple_string<dynamic_string_data, wchar_t, 0, true> wdynamic_string0;
typedef simple_string<dynamic_string_data, wchar_t, 32, true> wdynamic_string;

}
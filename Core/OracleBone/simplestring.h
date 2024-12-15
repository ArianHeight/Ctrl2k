#pragma once
#include "stringutil.h"

namespace obn
{

/*

This is the base string struct that makes up the interface for all the different basic custom string types
within the obn namespace.
Most of the logic for fixed-size strings are here, apart from the constructor(as well as the actual data defs).
Some of the logic for dynamic-size strings are here, namely every constexpr check for dyn marks dynamic string specific code.

*/
template <template <typename U, size_t N> typename datatype, typename chartype, size_t num, bool dyn = false>
class simple_string
{
    // templated self friend
    // do that care to not access data across different chartypes though
    template <template <typename U2, size_t N2> typename datatype2, typename chartype2, size_t num2, bool dyn2>
    friend class simple_string;

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

    inline bool empty() const { return internal_data.len == 0; }
    inline void shrink_to_fit() { if constexpr(dyn) internal_data.shrink_to_fit(); }

    inline void reset_error() { internal_data.error = false; }

    void clear()
    {
        internal_data.len = 0;
        internal_data.error = false;
        internal_data.buf[0] = 0;
    }

    void copy(const chartype* str, size_t str_len)
    {
        if constexpr(dyn)
        {
            internal_data.len = str_len;
            internal_data.increase_capacity_erase_data(internal_data.len + 1);
        }
        else
        {
            internal_data.error = str_len >= internal_data.max_len;
            internal_data.len = internal_data.error ? internal_data.max_len : str_len;
        }

        // use the faster mem cpy but without the trailing 0
        internal_data.error = mem_copy(internal_data.buf, str, internal_data.len * sizeof(chartype)) != 0 || internal_data.error;
        internal_data.buf[internal_data.len] = 0; // add trailing 0 back in very important
    }

    inline void copy(const chartype* str)
    {
        if constexpr(dyn)
            copy(str, string_len(str));
        else
            copy(str, string_nlen(str, internal_data.capacity));
    }

    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline void copy(const simple_string<datatype2, chartype, num2, dyn2>& other)
    {
        copy(other.internal_data.buf, other.internal_data.len);
    }

    // no move for c strings because that is kind of dangerous
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    void move(simple_string<datatype2, chartype, num2, dyn2>&& other)
    {
        if constexpr(dyn && dyn2) // only with dynamic strings that own heap memory
        {
            internal_data.delete_memory();
            if(other.internal_data.owns_memory())
            {
                internal_data.error = other.internal_data.error;
                internal_data.len = other.internal_data.len;
                internal_data.capacity = other.internal_data.capacity;
                internal_data.buf = other.internal_data.buf;
                other.internal_data.reset_values();
            }
            else
            {
                internal_data.reset_values();
            }
        }
        else
        {
            copy(other);
        }
    }

    void append(const chartype* str, size_t str_len)
    {
        if(str_len == 0)
            return;
        if constexpr(dyn)
        {
            const size_t new_capacity = internal_data.len + str_len;
            if(new_capacity >= internal_data.capacity)
            {
                const size_t tentative_capacity = internal_data.capacity << 1;
                internal_data.change_capacity(new_capacity > tentative_capacity ? new_capacity : tentative_capacity);
            }
        }
        else
        {
            internal_data.error = internal_data.len + str_len >= internal_data.max_len;
            str_len = internal_data.error ? internal_data.max_len - internal_data.len : str_len;
            if(str_len == 0)
                return;
        }

        // use the faster mem cpy but without the trailing 0
        internal_data.error = mem_copy(&internal_data.buf[internal_data.len], str, str_len * sizeof(chartype)) != 0 || internal_data.error;
        internal_data.len = internal_data.len + str_len;
        internal_data.buf[internal_data.len] = 0; // add trailing 0 back in very important
    }
    
    inline void append(const chartype* str)
    {
        if constexpr(dyn)
            append(str, string_len(str));
        else
            append(str, string_nlen(str, internal_data.capacity));
    }

    inline void append(chartype c)
    {
        chartype temp[2] = { c, 0 };
        append(temp, 1);
    }

    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline void append(const simple_string<datatype2, chartype, num2, dyn2>& other)
    {
        append(other.internal_data.buf, other.internal_data.len);
    }

    simple_string() {}
    simple_string(const chartype* str) { copy(str); }
    simple_string(const selftype& other) { copy(other); } // not very efficient
    simple_string(selftype&& other) { move(std::move(other)); } // also not very efficient

    // copy c string assignment
    inline selftype& operator=(const chartype* str) { copy(str); return *this; }
    // copy assignment
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline selftype& operator=(const simple_string<datatype2, chartype, num2, dyn2>& other) { copy(other); return *this; }
    // move assignment
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline selftype& operator=(simple_string<datatype2, chartype, num2, dyn2>&& other) { move(std::move(other)); return *this; }

    // wrappers for append
    inline selftype& operator+=(const chartype* str) { append(str); return *this; }
    inline selftype& operator+=(chartype c) { append(c); return *this; }
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline selftype& operator+=(const simple_string<datatype2, chartype, num2, dyn2>& other) { append(other); return *this; }

    // equals c string
    inline bool operator==(const chartype* str) const { return string_ncmp(internal_data.buf, str, internal_data.capacity) == 0; }
    // equals other simple_string
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline bool operator==(const simple_string<datatype2, chartype, num2, dyn2>& other) const
    {
        return internal_data.len == other.internal_data.len && string_ncmp(internal_data.buf, other.internal_data.buf, internal_data.len + 1) == 0;
    }

    inline chartype operator[](size_t idx) const
    {
#ifdef _DEBUG
        assertIndex(idx, internal_data.len);
#endif
        return internal_data.buf[idx];
    }

    inline chartype& operator[](size_t idx)
    {
#ifdef _DEBUG
        assertIndex(idx, internal_data.len);
#endif
        return internal_data.buf[idx];
    }
};

}
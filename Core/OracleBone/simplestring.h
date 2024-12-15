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
struct simple_string
{
    // templated self friend
    // do that care to not access data across different chartypes though
    template <template <typename U2, size_t N2> typename datatype2, typename chartype2, size_t num2, bool dyn2>
    friend struct simple_string;

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
    inline void shrink_to_fit() const { if constexpr(dyn) internal_data.shrink_capacity(); }

    void clear()
    {
        internal_data.len = 0;
        internal_data.error = false;
        internal_data.buf[0] = 0;
    }

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

    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    void copy(const simple_string<datatype2, chartype, num2, dyn2>& other)
    {
        if constexpr(dyn)
            internal_data.increase_capacity(other.internal_data.len + 1);

        string_ncopy(internal_data.buf, other.internal_data.buf, internal_data.capacity);
        internal_data.error = other.internal_data.error;
        internal_data.len = other.internal_data.len;
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

    simple_string() {}
    simple_string(const chartype* str) { copy(str); }

    // copy c string assignment
    inline selftype& operator=(const chartype* str) { copy(str); return *this; }
    // copy assignment
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline selftype& operator=(const simple_string<datatype2, chartype, num2, dyn2>& other) { copy(other); return *this; }
    // move assignment
    template <template <typename U2, size_t N2> typename datatype2, size_t num2, bool dyn2>
    inline selftype& operator=(simple_string<datatype2, chartype, num2, dyn2>&& other) { move(std::move(other)); return *this; }

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
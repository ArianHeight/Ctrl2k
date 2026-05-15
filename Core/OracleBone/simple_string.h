#pragma once
#include "borrowed_string.h"

namespace obn
{

/*

This is the base string struct that makes up the interface for all the different basic custom string types
within the obn namespace.
Most of the logic for fixed-size strings are here, apart from the constructor(as well as the actual data defs).
Some of the logic for dynamic-size strings are here, namely every constexpr check for _dyn marks dynamic string specific code.

*/
template <template <typename U, size_t N> typename S, typename T, size_t _num, bool _dyn = false>
requires (TYPE_CHAR<T>)
class simple_string
{
    // templated self friend
    // do that care to not access data across different Ts though
    // TODO this requires probably should be same_as T, but currently we don't have a custom impl, only std
    template <template <typename U2, size_t N2> typename S2, typename T2, size_t _num2, bool _dyn2>
    requires (TYPE_CHAR<T2>)
    friend class simple_string;

private:
    using fulltype = S<T, _num>;
    using selftype = simple_string<S, T, _num, _dyn>;
    using viewtype = borrowed_string<T>;

    fulltype internal_data;

public:
    inline bool has_error() const { return internal_data.error; }
    inline const T* c_str() const { return internal_data.buf; }
    inline const T* data() const { return internal_data.buf; }
    inline size_t length() const { return internal_data.len; }
    inline size_t size() const { return internal_data.len; }
    inline size_t capacity() const{ return internal_data.capacity; }
    inline size_t max_length() const { return internal_data.max_len; }

    inline bool empty() const { return internal_data.len == 0; }
    inline void shrink_to_fit() { if constexpr(_dyn) internal_data.shrink_to_fit(); }

    inline void reset_error() { internal_data.error = false; }

    void clear()
    {
        internal_data.len = 0;
        internal_data.error = false;
        internal_data.buf[0] = 0;
    }

    void replace(T old_c, T new_c)
    {
        const T* const end_ptr = &internal_data.buf[internal_data.len];
        for(T* buf_ptr = internal_data.buf; buf_ptr < end_ptr; ++buf_ptr)
            *buf_ptr = (*buf_ptr == old_c) ? new_c : *buf_ptr;
    }

    void copy(const T* str, size_t str_len)
    {
        if((T*)internal_data.buf == str)
        {
            return;
        }
        if constexpr(_dyn)
        {
            internal_data.len = str_len;
            internal_data.increase_capacity_erase_data(internal_data.len + 1);
        }
        else
        {
            internal_data.error = str_len >= internal_data.max_len;
            internal_data.len = internal_data.error ? internal_data.max_len : str_len;
        }

        ptr_assert(str);
        // use the faster mem cpy but without the trailing 0
        internal_data.error = mem_copy(internal_data.buf, str, internal_data.len * sizeof(T)) != 0 || internal_data.error;
        internal_data.buf[internal_data.len] = 0; // add trailing 0 back in very important
    }
    inline void copy(const T* str) { copy(str, string_nlen(str, internal_data.max_len)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline void copy(const simple_string<S2, T, _num2, _dyn2>& other) { copy(other.internal_data.buf, other.internal_data.len); }

    // no move for c strings because that is kind of dangerous
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    void move(simple_string<S2, T, _num2, _dyn2>&& other)
    {
        if((T*)internal_data.buf == (T*)other.internal_data.buf)
        {
            return;
        }
        if constexpr(_dyn && _dyn2) // only with dynamic strings that own heap memory
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

    void append(const T* str, size_t str_len)
    {
        if(str_len == 0)
            return;
        if constexpr(_dyn)
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
            internal_data.error = internal_data.len + str_len > internal_data.max_len;
            str_len = internal_data.error ? internal_data.max_len - internal_data.len : str_len;
            if(str_len == 0)
                return;
        }

        ptr_assert(str);
        // use the faster mem cpy but without the trailing 0
        internal_data.error = mem_copy(&internal_data.buf[internal_data.len], str, str_len * sizeof(T)) != 0 || internal_data.error;
        internal_data.len = internal_data.len + str_len;
        internal_data.buf[internal_data.len] = 0; // add trailing 0 back in very important
    }
    inline void append(const T* str) { append(str, string_nlen(str, internal_data.max_len)); }
    inline void append(T c) { T temp[2] = { c, 0 }; append(temp, 1); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline void append(const simple_string<S2, T, _num2, _dyn2>& other) { append(other.internal_data.buf, other.internal_data.len); }

    inline bool equals(const T* str, size_t str_len) const
    {
        return internal_data.len == str_len && (internal_data.buf == str || string_neq(internal_data.buf, str, internal_data.len));
    }
    inline bool equals(const T* str) const
    {
        ptr_assert(str);
        return equals(str, string_nlen(str, internal_data.len + 1));
    }

    inline selftype substring(size_t idx, size_t count) const
    {
        internal_full_range_assert(idx, count, internal_data.len);
        return selftype(&internal_data.buf[idx], count);
    }

    inline selftype substring(size_t count) const
    {
        full_internal_full_range_assert(count, internal_data.len);
        return selftype(internal_data.buf, count);
    }

    inline viewtype subview(size_t idx, size_t count) const
    {
        internal_full_range_assert(idx, count, internal_data.len);
        return viewtype(&internal_data.buf[idx], count);
    }

    inline viewtype subview(size_t count) const
    {
        full_internal_full_range_assert(count, internal_data.len);
        return viewtype(internal_data.buf, count);
    }

    inline bool starts_with(T c) const { return internal_data.len > 0 && internal_data.buf[0] == c; }
    inline bool starts_with(const T* str, size_t str_len) const
    {
        ptr_assert(str);
        return str_len <= internal_data.len && string_neq(internal_data.buf, str, str_len);
    }
    inline bool starts_with(const T* str) const { return starts_with(str, string_len(str)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline bool starts_with(const simple_string<S2, T, _num2, _dyn2>& other) const
    {
        return starts_with(other.internal_data.buf, other.internal_data.len);
    }

    inline bool ends_with(T c) const { return internal_data.len > 0 && internal_data.buf[internal_data.len - 1] == c; }
    inline bool ends_with(const T* str, size_t str_len) const
    {
        ptr_assert(str);
        return str_len <= internal_data.len && string_neq(&internal_data.buf[internal_data.len - str_len], str, str_len);
    }
    inline bool ends_with(const T* str) const { return ends_with(str, string_len(str)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline bool ends_with(const simple_string<S2, T, _num2, _dyn2>& other) const
    {
        return ends_with(other.internal_data.buf, other.internal_data.len);
    }

    inline size_t find(const T* str, size_t str_len, size_t start = 0) const
    {
        ptr_assert(str);
        return string_nfind(internal_data.buf, internal_data.len, str, str_len, start);
    }
    inline size_t find(const T* str) const { return find(str, string_len(str)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t find(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = 0) const
    {
        return find(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t find(const viewtype& other, size_t start = 0) const { return find(other.c_str(), other.length(), start); }
    inline size_t find(const T chr, size_t start = 0) const
    {
        return string_nfind(internal_data.buf, internal_data.len, chr, start);
    }

    inline size_t rfind(const T* str, size_t str_len, size_t start = INVALID_SIZE_T) const
    {
        ptr_assert(str);
        return string_nrfind(internal_data.buf, internal_data.len, str, str_len, start);
    }
    inline size_t rfind(const T* str) const { return rfind(str, string_len(str)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t rfind(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = INVALID_SIZE_T) const
    {
        return rfind(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t rfind(const viewtype& other, size_t start = INVALID_SIZE_T) const { return rfind(other.c_str(), other.length(), start); }
    inline size_t rfind(const T chr, size_t start = INVALID_SIZE_T) const
    {
        return string_nrfind(internal_data.buf, internal_data.len, chr, start);
    }

    inline size_t find_first_of(const T* charset, size_t charset_len, size_t start = 0) const
    {
        ptr_assert(charset);
        return string_nfind_first_of(internal_data.buf, internal_data.len, charset, charset_len, start);
    }
    inline size_t find_first_of(const T* charset) const { return find_first_of(charset, string_len(charset)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t find_first_of(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = 0) const
    {
        return find_first_of(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t find_first_of(const viewtype& other, size_t start = 0) const { return find_first_of(other.c_str(), other.length(), start); }

    inline size_t find_first_not_of(const T* charset, size_t charset_len, size_t start = 0) const
    {
        ptr_assert(charset);
        return string_nfind_first_not_of(internal_data.buf, internal_data.len, charset, charset_len, start);
    }
    inline size_t find_first_not_of(const T* charset) const { return find_first_not_of(charset, string_len(charset)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t find_first_not_of(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = 0) const
    {
        return find_first_not_of(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t find_first_not_of(const viewtype& other, size_t start = 0) const { return find_first_not_of(other.c_str(), other.length(), start); }

    inline size_t find_last_of(const T* charset, size_t charset_len, size_t start = INVALID_SIZE_T) const
    {
        ptr_assert(charset);
        return string_nfind_last_of(internal_data.buf, internal_data.len, charset, charset_len, start);
    }
    inline size_t find_last_of(const T* charset) const { return find_last_of(charset, string_len(charset)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t find_last_of(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = INVALID_SIZE_T) const
    {
        return find_last_of(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t find_last_of(const viewtype& other, size_t start = INVALID_SIZE_T) const
    {
        return find_last_of(other.c_str(), other.length(), start);
    }

    inline size_t find_last_not_of(const T* charset, size_t charset_len, size_t start = INVALID_SIZE_T) const
    {
        ptr_assert(charset);
        return string_nfind_last_not_of(internal_data.buf, internal_data.len, charset, charset_len, start);
    }
    inline size_t find_last_not_of(const T* charset) const { return find_last_not_of(charset, string_len(charset)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline size_t find_last_not_of(const simple_string<S2, T, _num2, _dyn2>& other, size_t start = INVALID_SIZE_T) const
    {
        return find_last_not_of(other.internal_data.buf, other.internal_data.len, start);
    }
    inline size_t find_last_not_of(const viewtype& other, size_t start = INVALID_SIZE_T) const
    {
        return find_last_not_of(other.c_str(), other.length(), start);
    }

    // constructors
    simple_string() {}
    simple_string(const T* str) { copy(str); }
    simple_string(const T* str, size_t str_len) { copy(str, str_len); }
    simple_string(const viewtype& other) { copy(other.data(), other.length()); }
    simple_string(const selftype& other) { copy(other); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    simple_string(const simple_string<S2, T, _num2, _dyn2>& other) { copy(other); }
    simple_string(selftype&& other) { move(std::move(other)); }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    simple_string(simple_string<S2, T, _num2, _dyn2>&& other) { move(std::move(other)); }

    // copy c string assignment
    inline selftype& operator=(const T* str) { copy(str); return *this; }
    // copy view assignment
    inline selftype& operator=(const viewtype& other) { copy(other.data(), other.length()); return *this; }
    // copy assignment self
    inline selftype& operator=(const selftype& other) { copy(other); return *this; }
    // copy assignment general
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline selftype& operator=(const simple_string<S2, T, _num2, _dyn2>& other) { copy(other); return *this; }
    // move assignment self
    inline selftype& operator=(selftype&& other) noexcept { move(std::move(other)); return *this; }
    // move assignment general
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline selftype& operator=(simple_string<S2, T, _num2, _dyn2>&& other) { move(std::move(other)); return *this; }

    // wrappers for append
    inline selftype& operator+=(const T* str) { append(str); return *this; }
    inline selftype& operator+=(T c) { append(c); return *this; }
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline selftype& operator+=(const simple_string<S2, T, _num2, _dyn2>& other) { append(other); return *this; }
    inline selftype& operator+=(const viewtype& other) { append(other.data(), other.length()); return *this; }

    // equals c string
    inline bool operator==(const T* str) const { return equals(str); }
    // equals view
    inline bool operator==(const viewtype& other) const { return equals(other.data(), other.length()); }
    // equals other simple_string
    template <template <typename U2, size_t N2> typename S2, size_t _num2, bool _dyn2>
    inline bool operator==(const simple_string<S2, T, _num2, _dyn2>& other) const
    {
        return equals(other.internal_data.buf, other.internal_data.len);
    }

    inline T operator[](size_t idx) const
    {
        index_assert(idx, internal_data.len);
        return internal_data.buf[idx];
    }

    inline T& operator[](size_t idx)
    {
        index_assert(idx, internal_data.len);
        return internal_data.buf[idx];
    }

    inline viewtype view() const { return viewtype(internal_data.buf, internal_data.len); }
};

}
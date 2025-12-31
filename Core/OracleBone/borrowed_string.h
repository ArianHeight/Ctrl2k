#pragma once
#include "string_util.h"

namespace obn
{

/*

This is essentially std::string_view, a "borrowed" string that does not own the string data it is pointed to.

*/
template <typename chartype>
class borrowed_string : public unmoveable
{
private:
    using selftype = borrowed_string<chartype>;

    inline static chartype zero_val = 0; // please don't touch this

    const chartype* m_data;
    size_t m_len;

public:
    inline const chartype* c_str() const { return m_data; }
    inline const chartype* data() const { return m_data; }
    inline size_t length() const { return m_len; }
    inline size_t size() const { return m_len; }

    inline bool empty() const { return m_len == 0; }

    inline void clear()
    {
        m_data = &zero_val;
        m_len = 0;
    }

    inline void set(const chartype* str, size_t str_len)
    {
        ptr_assert(str);
        m_data = str;
        m_len = str_len;
    }

    inline void set(const chartype* str)
    {
        ptr_assert(str);
        m_data = str;
        m_len = string_len(str); // this seems dangerous, unbounded length check
    }

    inline void set(const selftype& other)
    {
        m_data = other.m_data;
        m_len = other.m_len;
    }

    inline bool equals(const chartype* str, size_t len) const
    {
        return m_len == len && (m_data == str || string_neq(m_data, str, m_len));
    }
    inline bool equals(const chartype* str) const
    {
        ptr_assert(str);
        return equals(str, string_nlen(str, m_len + 1));
    }

    inline selftype substring(size_t idx, size_t count) const
    {
        index_assert(idx, m_len);
        index_assert(idx + count, m_len + 1);
        return selftype(&m_data[idx], count);
    }

    inline selftype substring(size_t count) const
    {
        index_assert(count, m_len + 1);
        return selftype(m_data, count);
    }

    inline bool starts_with(chartype c) const { return m_len > 0 && m_data[0] == c; }
    inline bool starts_with(const chartype* str, size_t str_len) const
    {
        ptr_assert(str);
        return str_len <= m_len && string_neq(m_data, str, str_len);
    }
    inline bool starts_with(const chartype* str) const { return starts_with(str, string_len(str)); }
    inline bool starts_with(const selftype& other) const { return starts_with(other.m_data, other.m_len); }

    inline bool ends_with(chartype c) const { return m_len > 0 && m_data[m_len - 1] == c; }
    inline bool ends_with(const chartype* str, size_t str_len) const
    {
        ptr_assert(str);
        return str_len <= m_len && string_neq(&m_data[m_len - str_len], str, str_len);
    }
    inline bool ends_with(const chartype* str) const { return ends_with(str, string_len(str)); }
    inline bool ends_with(const selftype& other) const { return ends_with(other.m_data, other.m_len); }

    inline size_t find(const chartype* str, size_t str_len, size_t start = 0) const
    {
        ptr_assert(str);
        return string_nfind(m_data, m_len, str, str_len, start);
    }
    inline size_t find(const chartype* str) const { return find(str, string_len(str)); }
    inline size_t find(const selftype& other, size_t start = 0) const { return find(other.m_data, other.m_len, start); }
    inline size_t find(const chartype chr, size_t start = 0) const
    {
        return string_nfind(m_data, m_len, chr, start);
    }

    inline size_t rfind(const chartype* str, size_t str_len, size_t start) const
    {
        ptr_assert(str);
        return string_nrfind(m_data, m_len, str, str_len, start);
    }
    inline size_t rfind(const chartype* str, size_t str_len) const { return rfind(str, str_len, m_len); }
    inline size_t rfind(const chartype* str) const { return rfind(str, string_len(str)); }
    inline size_t rfind(const selftype& other, size_t start) const { return rfind(other.m_data, other.m_len, start); }
    inline size_t rfind(const selftype& other) const { return rfind(other, m_len); }
    inline size_t rfind(const chartype chr, size_t start) const
    {
        return string_nrfind(m_data, m_len, chr, start);
    }
    inline size_t rfind(const chartype chr) const { return rfind(chr, m_len); }
    
    // constructors
    borrowed_string() : m_data(&zero_val), m_len(0) {}
    borrowed_string(const chartype* str) { set(str); } // this seems dangerous
    borrowed_string(const chartype* str, size_t str_len) { set(str, str_len); }
    borrowed_string(const selftype& other) : m_data(other.m_data), m_len(other.m_len) {}

    // copy c string assignment
    inline selftype& operator=(const chartype* str) { set(str); return *this; }
    // copy assignment self
    inline selftype& operator=(const selftype& other) { set(other); return *this; }

    // equals c string
    inline bool operator==(const chartype* str) const { return equals(str); }
    // equals other borrowed_string
    inline bool operator==(const selftype& other) const { return equals(other.m_data, other.m_len); }

    inline chartype operator[](size_t idx) const
    {
        index_assert(idx, m_len);
        return m_data[idx];
    }
};

typedef borrowed_string<char> view_string;
typedef borrowed_string<wchar_t> view_wstring;

}
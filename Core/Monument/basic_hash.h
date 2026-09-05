#pragma once
#include <stdint.h>

union hash32_t
{
    uint8_t arr8[4];
    uint32_t val;

    hash32_t() = default;
    hash32_t(uint32_t a) : val(a) {}

    inline bool operator==(const hash32_t& other) const { return val == other.val; }
    inline bool operator!=(const hash32_t& other) const { return !(*this == other); }
    inline bool operator<(const hash32_t& other) const { return val < other.val; }
    inline bool operator<=(const hash32_t& other) const { return (*this == other) || (*this < other); }

    static inline hash32_t get_zero() { return 0; }
};

union hash64_t
{
    uint8_t arr8[8];
    struct sub32_t
    {
        uint32_t a;
        uint32_t b;
    };
    sub32_t sub32;
    uint32_t arr32[2];
    uint64_t val;

    hash64_t() = default;
    constexpr hash64_t(uint64_t a) : val(a) {}

    inline bool operator==(const hash64_t& other) const { return val == other.val; }
    inline bool operator!=(const hash64_t& other) const { return !(*this == other); }
    inline bool operator<(const hash64_t& other) const { return val < other.val; }
    inline bool operator<=(const hash64_t& other) const { return (*this == other) || (*this < other); }

    static consteval hash64_t get_zero() { return 0; }
};
constexpr hash64_t INVALID_HASH64 = hash64_t::get_zero();

union hash128_t
{
    uint8_t arr8[16];
    struct sub32_t
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
    };
    sub32_t sub32;
    uint32_t arr32[4];
    struct sub64_t
    {
        uint64_t a;
        uint64_t b;
    };
    sub64_t sub64;
    uint64_t arr64[2];

    hash128_t() = default;
    hash128_t(uint64_t a, uint64_t b) : sub64{ a, b } {}

    inline bool operator==(const hash128_t& other) const { return sub64.a == other.sub64.a && sub64.b == other.sub64.b; }
    inline bool operator!=(const hash128_t& other) const { return !(*this == other); }
    inline bool operator<(const hash128_t& other) const
    {
        return (sub64.a != other.sub64.a) ? sub64.a < other.sub64.a : sub64.b < other.sub64.b;
    }
    inline bool operator<=(const hash128_t& other) const { return (*this == other) || (*this < other); }

    static inline hash128_t get_zero() { return hash128_t{ 0, 0 }; }
};

union hash256_t
{
    uint8_t arr8[32];
    struct sub32_t
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
        uint32_t e;
        uint32_t f;
        uint32_t g;
        uint32_t h;
    };
    sub32_t sub32;
    uint32_t arr32[8];
    struct sub64_t
    {
        uint64_t a;
        uint64_t b;
        uint64_t c;
        uint64_t d;
    };
    sub64_t sub64;
    uint64_t arr64[4];

    hash256_t() = default;
    hash256_t(uint64_t a, uint64_t b, uint64_t c, uint64_t d) : sub64{ a, b, c, d } {}

    inline bool operator==(const hash256_t& other) const
    {
        return sub64.a == other.sub64.a && sub64.b == other.sub64.b && sub64.c == other.sub64.c && sub64.d == other.sub64.d;
    }
    inline bool operator!=(const hash256_t& other) const { return !(*this == other); }
    inline bool operator<(const hash256_t& other) const
    {
        if(sub64.a != other.sub64.a)
            return sub64.a < other.sub64.a;
        if(sub64.b != other.sub64.b)
            return sub64.b < other.sub64.b;
        if(sub64.c != other.sub64.c)
            return sub64.c < other.sub64.c;
        return sub64.d < other.sub64.d;
    }
    inline bool operator<=(const hash256_t& other) const { return (*this == other) || (*this < other); }

    static inline hash256_t get_zero() { return hash256_t{ 0, 0, 0, 0 }; }
};

hash64_t simple_hash(c_string str, size_t len, hash64_t seed = hash64_t::get_zero());
hash64_t simple_hash(wc_string str, size_t len, hash64_t seed = hash64_t::get_zero());

uint32_t checksum32(c_string str, size_t len);
uint32_t checksum32(wc_string str, size_t len);
uint64_t checksum64(c_string str, size_t len);
uint64_t checksum64(wc_string str, size_t len);

template <typename T>
struct _generic_hash_functor
{
    inline hash64_t operator()(const T& val) const
    {
        compile_assert_msg(false, "please implement a specific hash function for this data type");
        return INVALID_HASH64;
    }
};

// DONOT use this outside of this file pls
#define DEFAULT_HASH_DEF(type) \
template <>\
struct _generic_hash_functor<type>\
{\
    inline hash64_t operator()(const type& val) const\
    {\
        return (size_t)val;\
    }\
};

DEFAULT_HASH_DEF(signed char);
DEFAULT_HASH_DEF(short);
DEFAULT_HASH_DEF(int);
DEFAULT_HASH_DEF(long);
DEFAULT_HASH_DEF(long long);

DEFAULT_HASH_DEF(unsigned char);
DEFAULT_HASH_DEF(unsigned short);
DEFAULT_HASH_DEF(unsigned int);
DEFAULT_HASH_DEF(unsigned long);
DEFAULT_HASH_DEF(unsigned long long);

DEFAULT_HASH_DEF(float);
DEFAULT_HASH_DEF(double);
DEFAULT_HASH_DEF(long double);

DEFAULT_HASH_DEF(char);
DEFAULT_HASH_DEF(wchar_t);
DEFAULT_HASH_DEF(char8_t);
DEFAULT_HASH_DEF(char16_t);
DEFAULT_HASH_DEF(char32_t);

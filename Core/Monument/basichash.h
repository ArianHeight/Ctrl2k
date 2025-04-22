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
    hash64_t(uint64_t a) : val(a) {}

    inline bool operator==(const hash64_t& other) const { return val == other.val; }
    inline bool operator!=(const hash64_t& other) const { return !(*this == other); }
    inline bool operator<(const hash64_t& other) const { return val < other.val; }
    inline bool operator<=(const hash64_t& other) const { return (*this == other) || (*this < other); }

    static inline hash64_t get_zero() { return 0; }
};

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

hash32_t checksum32(const void* data, size_t len, hash32_t seed = hash32_t::get_zero());
hash64_t checksum64(const void* data, size_t len, hash64_t seed = hash64_t::get_zero());
hash128_t checksum128(const void* data, size_t len, hash128_t seed = hash128_t::get_zero());
hash256_t checksum256(const void* data, size_t len, hash256_t seed = hash256_t::get_zero());

template <typename T>
inline hash32_t checksum32(const T& obj) { return checksum32(&obj, sizeof(obj)); }
template <typename T>
inline hash64_t checksum64(const T& obj) { return checksum64(&obj, sizeof(obj)); }
template <typename T>
inline hash128_t checksum128(const T& obj) { return checksum128(&obj, sizeof(obj)); }
template <typename T>
inline hash256_t checksum256(const T& obj) { return checksum256(&obj, sizeof(obj)); }
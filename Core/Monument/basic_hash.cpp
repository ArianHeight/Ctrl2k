#include "Monument.h"

static inline uint32_t make_big_endian_4bytes(const uint8_t arr[4])
{
    return ((uint32_t)arr[0] << 24) | ((uint32_t)arr[1] << 16) | ((uint32_t)arr[2] << 8) | (uint32_t)arr[3];
}

static inline uint64_t make_big_endian_8bytes(const uint8_t arr[8])
{
    return ((uint64_t)arr[0] << 56) | ((uint64_t)arr[1] << 48) | ((uint64_t)arr[2] << 40) | ((uint64_t)arr[3] << 32) |
        ((uint64_t)arr[4] << 24) | ((uint64_t)arr[5] << 16) | ((uint64_t)arr[6] << 8) | (uint64_t)arr[7];
}

#if 0

hash32_t checksum32(const void* data, size_t len, hash32_t seed)
{
    const size_t numloops = len >> NUM_BITS_FOR_4;
    const size_t endloop = numloops << NUM_BITS_FOR_4;

    const uint8_t* arr = (uint8_t*)data;
    for(size_t i = 0; i < numloops; i++)
    {
        seed.val += make_big_endian_4bytes(&arr[i << NUM_BITS_FOR_4]);
    }

    uint8_t temp[4] = { 0, 0, 0, 0 };
    for(size_t i = endloop; i < len; i++)
    {
        temp[i - endloop] = arr[i];
    }
    seed.val += make_big_endian_4bytes(temp);

    return seed;
}

hash64_t checksum64(const void* data, size_t len, hash64_t seed)
{
    const size_t numloops = len >> NUM_BITS_FOR_8;
    const size_t endloop = numloops << NUM_BITS_FOR_8;

    const uint8_t* arr = (uint8_t*)data;
    for(size_t i = 0; i < numloops; i++)
    {
        seed.val += make_big_endian_8bytes(&arr[i << NUM_BITS_FOR_8]);
    }

    uint8_t temp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for(size_t i = endloop; i < len; i++)
    {
        temp[i - endloop] = arr[i];
    }
    seed.val += make_big_endian_8bytes(temp);

    return seed;
}

hash128_t checksum128(const void* data, size_t len, hash128_t seed)
{
    const size_t numloops = len >> NUM_BITS_FOR_8;
    const size_t endloop = numloops << NUM_BITS_FOR_8;

    const uint8_t* arr = (uint8_t*)data;
    for(size_t i = 0; i < numloops; i++)
    {
        seed.arr64[i & MASK_1_BIT] += make_big_endian_8bytes(&arr[i << NUM_BITS_FOR_8]);
    }

    uint8_t temp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for(size_t i = endloop; i < len; i++)
    {
        temp[i - endloop] = arr[i];
    }
    seed.arr64[numloops & MASK_1_BIT] += make_big_endian_8bytes(temp);

    return seed;
}

hash256_t checksum256(const void* data, size_t len, hash256_t seed)
{
    const size_t numloops = len >> NUM_BITS_FOR_8;
    const size_t endloop = numloops << NUM_BITS_FOR_8;

    const uint8_t* arr = (uint8_t*)data;
    for(size_t i = 0; i < numloops; i++)
    {
        seed.arr64[i & MASK_2_BIT] += make_big_endian_8bytes(&arr[i << NUM_BITS_FOR_8]);
    }

    uint8_t temp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for(size_t i = endloop; i < len; i++)
    {
        temp[i - endloop] = arr[i];
    }
    seed.arr64[numloops & MASK_2_BIT] += make_big_endian_8bytes(temp);

    return seed;
}

#endif

// perhaps this could live in math instead? Or this can just stay here and be used for simple hashing
static const uint64_t hash_primes[] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
static constexpr size_t NUM_HASH_PRIMES = sizeof(hash_primes) / sizeof(uint64_t);
compile_assert(sizeof(hash64_t) == sizeof(uint64_t));

template <typename T>
requires (TYPE_CHAR<T> && sizeof(T) <= sizeof(hash64_t))
static hash64_t simple_hash_string(const T* str, size_t len, hash64_t seed)
{
    constexpr size_t CHARS_PER_HASH = sizeof(hash64_t) / sizeof(T);
    compile_assert(CHARS_PER_HASH > 0);

    constexpr size_t BITS_PER_CHAR = NUM_BITS_PER_BYTE * sizeof(T);

    size_t hash_index = 0;
    for(size_t i = 0; i < len; i += CHARS_PER_HASH)
    {
        uint64_t value = 0;
        for(size_t j = 0; j < CHARS_PER_HASH && i + j < len; j++)
        {
            value |= ((uint64_t)str[i + j]) << (BITS_PER_CHAR * j);
        }
        seed.val += value * hash_primes[hash_index % NUM_HASH_PRIMES]; // could use bitshifting rather than modulus
        ++hash_index;
    }

    seed.val += len;

    return seed;
}

hash64_t simple_hash(c_string str, size_t len, hash64_t seed)
{
    return simple_hash_string(str, len, seed);
}

hash64_t simple_hash(wc_string str, size_t len, hash64_t seed)
{
    return simple_hash_string(str, len, seed);
}

template <typename T, typename V>
requires (TYPE_CHAR<T> && TYPE_UNSIGNED_INTEGER<V> && sizeof(V) >= sizeof(uint32_t))
static V checksum_string(const T* str, size_t len)
{
    V checksum = 0;
    for(size_t i = 0; i < len; i++)
    {
        checksum += (V)str[i];
    }
    return checksum;
}

uint32_t checksum32(c_string str, size_t len) { return checksum_string<char, uint32_t>(str, len); }
uint32_t checksum32(wc_string str, size_t len) { return checksum_string<wchar_t, uint32_t>(str, len); }
uint64_t checksum64(c_string str, size_t len) { return checksum_string<char, uint64_t>(str, len); }
uint64_t checksum64(wc_string str, size_t len) { return checksum_string<wchar_t, uint64_t>(str, len); }

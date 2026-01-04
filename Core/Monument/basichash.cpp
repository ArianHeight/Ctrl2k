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

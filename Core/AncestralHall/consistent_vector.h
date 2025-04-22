#pragma once
#include <vector>
#include "Core/Monument/Monument.h"

namespace ahl
{

/*

A dynamically sized vector with guarunteed pointer/address consistency

*/

constexpr size_t CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS = 5ULL;
constexpr size_t CONSISTENT_VECTOR_INITIAL_CAPACITY = 1ULL << CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;

template<typename T>
class consistent_vector : public unmoveable
{
private:
    using selftype = consistent_vector<T>;

    std::vector<T*> m_buckets;
    size_t m_size;
    size_t m_capacity;

    size_t getBucketIndex(size_t& i) const
    {
        size_t digit = CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
        for(; i >> digit != 0; digit++);
        i &= ((1ULL << (digit - (digit == CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS ? 0ULL : 1ULL))) - 1ULL);
        return digit - CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
    }

    inline const T* getBucket(size_t& i) const
    {
        return m_buckets[getBucketIndex(i)];
    }

    inline T* getBucket(size_t& i)
    {
        return m_buckets[getBucketIndex(i)];
    }

    T* allocateAndGetBucket(size_t& i)
    {
        const size_t bucketIndex = getBucketIndex(i);
        for(size_t j = m_buckets.size(); j <= bucketIndex; j++)
        {
            const size_t newSize = 1ULL << (CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS + j);
            m_buckets.push_back(new T[newSize]);
            m_capacity += newSize;
        }
        return m_buckets[bucketIndex];
    }

public:
    consistent_vector() : m_size(0), m_capacity(CONSISTENT_VECTOR_INITIAL_CAPACITY), m_buckets{ new T[CONSISTENT_VECTOR_INITIAL_CAPACITY] } {}
    ~consistent_vector()
    {
        for(T* data : m_buckets)
        {
            delete[] data;
        }
        m_buckets.clear();
    }

    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }

    void pop_back()
    {
        assert(m_size > 0);
        m_size--;
    }

    void push_back(const T& elem)
    {
        size_t i = m_size;
        T* data = allocateAndGetBucket(i);
        data[i] = elem;
        m_size++;
    }

    void push_back(T&& elem)
    {
        size_t i = m_size;
        T* data = allocateAndGetBucket(i);
        data[i] = std::move(elem);
        m_size++;
    }

    T& emplace_back()
    {
        size_t i = m_size;
        T* data = allocateAndGetBucket(i);
        T& retVal = data[i];
        m_size++;
        return retVal;
    }

    //TODO maybe use memcpy?
    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            if(other.m_size > 0)
            {
                size_t i = other.m_size - 1ULL;
                allocateAndGetBucket(i);
                size_t bucketIdx = 0;
                size_t bucketSize = 1ULL << CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
                size_t localIdx = 0;
                for(i = 0; i < other.m_size; i++)
                {
                    m_buckets[bucketIdx][localIdx] = other.m_buckets[bucketIdx][localIdx];
                    localIdx++;
                    if(localIdx >= bucketSize)
                    {
                        bucketSize = bucketSize << 1;
                        localIdx = 0;
                    }
                }
            }
            m_size = other.m_size;
        }
        return *this;
    }

    inline const T& at(size_t i) const { const T* bucket = getBucket(i); return bucket[i]; }
    inline T& at(size_t i) { T* bucket = getBucket(i); return bucket[i]; }
    inline const T& operator[](size_t i) const { const T* bucket = getBucket(i); return bucket[i]; }
    inline T& operator[](size_t i) { T* bucket = getBucket(i); return bucket[i]; }
    inline const T& front() const { return m_buckets[0][0]; }
    inline T& front() { return m_buckets[0][0]; }
    inline const T& back() const { return at(m_size - 1); }
    inline T& back() { return at(m_size - 1); }
};

}
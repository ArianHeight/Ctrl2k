#pragma once
#include "dyn_vector.h"

namespace ahl::dyn
{

/*

A dynamically sized vector with guarunteed pointer/address consistency

The way the internal indexing works is we have blocks starting at size CONSISTENT_VECTOR_INITIAL_CAPACITY and doubling
for every new block. The first block and the second block are exceptions because they have the same capacity, this is
to allow for easier indexing where we can get the local index by removing the most significant bit, and the bucket index
by counting the number of digits to the most significant bit from CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS.

The memory is laid out as such:
block1: 32
block2: 32
block3: 64
block4: 128
block5: 256

this example has a total capacity of 256.

*/

constexpr size_t CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS = 5ULL;
constexpr size_t CONSISTENT_VECTOR_INITIAL_CAPACITY = 1ULL << CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
compile_assert(CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS > 1);

template <typename T>
class consistent_vector : public unmoveable
{
private:
    using selftype = consistent_vector<T>;

    ahl::dyn::vector<T*> m_buckets;
    size_t m_size;
    size_t m_capacity;

    inline size_t get_bucket_capacity(size_t bucket_index) const
    {
        const size_t offset = bucket_index - 1;
        return 1ULL << (CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS + (bucket_index == 0 ? 0 : offset));
    }

    size_t get_bucket_index(size_t& i) const
    {
        size_t digit = CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
        for(; i >> digit != 0; digit++);
        i &= ((1ULL << (digit - (digit == CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS ? 0ULL : 1ULL))) - 1ULL);
        return digit - CONSISTENT_VECTOR_INITIAL_CAPACITY_NUM_BITS;
    }

    inline const T* get_bucket(size_t& i) const
    {
        return m_buckets[get_bucket_index(i)];
    }

    inline T* get_bucket(size_t& i)
    {
        return m_buckets[get_bucket_index(i)];
    }

    T* allocate_and_get_bucket(size_t& i)
    {
        const size_t bucketIndex = get_bucket_index(i);
        for(size_t j = m_buckets.size(); j <= bucketIndex; j++)
        {
            const size_t newSize = get_bucket_capacity(j);
            m_buckets.push_back(new T[newSize]);
            m_capacity += newSize;
        }
        return m_buckets[bucketIndex];
    }

public:
    // maybe use memcpy?
    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            if(other.m_size > 0)
            {
                size_t i = other.m_size - 1ULL;
                allocate_and_get_bucket(i);
                
                size_t bucketIdx = 0;
                size_t bucketSize = get_bucket_capacity(bucketIdx);
                size_t localIdx = 0;
                for(i = 0; i < other.m_size; i++)
                {
                    m_buckets[bucketIdx][localIdx] = other.m_buckets[bucketIdx][localIdx];
                    localIdx++;
                    if(localIdx >= bucketSize)
                    {
                        localIdx = 0;
                        ++bucketIdx;
                        bucketSize = get_bucket_capacity(bucketIdx);
                    }
                }
            }
            m_size = other.m_size;
        }
        return *this;
    }

    selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            for(size_t i = 0; i < m_buckets.size(); ++i)
            {
                delete[] m_buckets[i];
                m_buckets[i] = nullptr;
            }
            m_buckets = std::move(other.m_buckets);
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_capacity = 0;
            other.m_size = 0;
        }
        return *this;
    }

    consistent_vector() : m_size(0), m_capacity(0) {}
    consistent_vector(const selftype& other) : m_size(0), m_capacity(0)
    {
        *this = other;
    }
    consistent_vector(selftype&& other) : m_size(0), m_capacity(0)
    {
        *this = std::move(other);
    }
    ~consistent_vector()
    {
        for(size_t i = 0; i < m_buckets.size(); ++i)
        {
            delete[] m_buckets[i];
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
        T* data = allocate_and_get_bucket(i);
        data[i] = elem;
        m_size++;
    }

    void push_back(T&& elem)
    {
        size_t i = m_size;
        T* data = allocate_and_get_bucket(i);
        data[i] = std::move(elem);
        m_size++;
    }

    T& emplace_back()
    {
        size_t i = m_size;
        T* data = allocate_and_get_bucket(i);
        T& retVal = data[i];
        m_size++;
        return retVal;
    }

    bool operator==(const selftype& other) const
    {
        if(m_size != other.m_size)
            return false;

        size_t bucketIdx = 0;
        size_t bucketSize = get_bucket_capacity(bucketIdx);
        size_t localIdx = 0;
        for(size_t i = 0; i < m_size; i++)
        {
            if(m_buckets[bucketIdx][localIdx] != other.m_buckets[bucketIdx][localIdx])
            {
                return false;
            }

            localIdx++;
            if(localIdx >= bucketSize)
            {
                localIdx = 0;
                ++bucketIdx;
                bucketSize = get_bucket_capacity(bucketIdx);
            }
        }

        return true;
    }

    inline const T& at(size_t i) const { const T* bucket = get_bucket(i); return bucket[i]; }
    inline T& at(size_t i) { T* bucket = get_bucket(i); return bucket[i]; }
    inline const T& operator[](size_t i) const { const T* bucket = get_bucket(i); return bucket[i]; }
    inline T& operator[](size_t i) { T* bucket = get_bucket(i); return bucket[i]; }
    inline const T& front() const { return m_buckets[0][0]; }
    inline T& front() { return m_buckets[0][0]; }
    inline const T& back() const { return at(m_size - 1); }
    inline T& back() { return at(m_size - 1); }
};

}
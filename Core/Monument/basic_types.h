#pragma once
#include <stdint.h>
#include <utility>

// null-terminated c-string, please do not use non null-terminated char* buffers if we're specifiying a c_string
typedef const char* c_string;
typedef const wchar_t* wc_string;

typedef uint8_t uint1B;
typedef int8_t int1B;
typedef uint16_t uint2B;
typedef int16_t int2B;
typedef uint32_t uint4B;
typedef int32_t int4B;
typedef uint64_t uint8B;
typedef int64_t int8B;

struct uncopyable
{
    uncopyable() = default;
    uncopyable(const uncopyable& other) = delete;
    uncopyable& operator=(const uncopyable& other) = delete;
};

struct unmoveable
{
    unmoveable() = default;
    unmoveable(unmoveable&& other) = delete;
    unmoveable& operator=(unmoveable&& other) = delete;
};

template <typename T>
struct data_bucket
{
private:
    using selftype = data_bucket<T>;

    size_t m_size;
    size_t m_capacity;
    T* m_data;

    void clear_data()
    {
        if(m_data)
        {
            delete[] m_data;
        }
        m_data = nullptr;
        m_capacity = 0;
        m_size = 0;
    }

    void move(selftype&& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = other.m_data;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

public:
    data_bucket() : m_size(0), m_capacity(0), m_data(nullptr) {}
    data_bucket(size_t bucket_size) : m_size(0), m_capacity(bucket_size), m_data(new T[bucket_size]) {}
    data_bucket(selftype&& other) noexcept { move(std::move(other)); }
    data_bucket(const selftype& other) = delete; // TODO maybe add this later?? Would have to do bounds check
    ~data_bucket() { clear_data(); }

    inline size_t& size() { return m_size; }
    inline const size_t& size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }
    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }

    inline selftype& operator=(selftype&& other) { move(std::move(other)); return *this; }
    inline selftype& operator=(const selftype& other) = delete; // TODO maybe add this later?? Would have to do bounds check

    inline const T& operator[](const size_t i) const { return m_data[i]; }
    inline T& operator[](const size_t i) { return m_data[i]; }
};
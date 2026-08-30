#pragma once
#include "Core/Monument/Monument.h"


namespace ahl::dyn
{

/*

A dynamically sized vector that does not have pointer stability

*/


template <typename T>
class vector
{
private:
    using selftype = vector<T>;

    T* m_data;
    size_t m_size;
    size_t m_capacity;

    inline void reallocate(size_t new_capacity)
    {
        m_capacity = new_capacity;
        T* old_data = m_data;
        m_data = new T[m_capacity];
        for(size_t i = 0; i < m_size; ++i)
        {
            m_data[i] = std::move(old_data[i]);
        }
        if(old_data)
        {
            delete[] old_data;
        }
    }

    inline void reallocate_if_needed(size_t largest_index)
    {
        if(largest_index < m_capacity)
            return;

        size_t new_capacity;
        for(new_capacity = 8; new_capacity <= largest_index; new_capacity <<= 1);
        reallocate(new_capacity);
    }

public:
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }
    inline bool empty() const { return m_size == 0; }

    inline void clear() { m_size = 0; }

    void reserve(size_t new_capacity)
    {
        if(new_capacity > m_capacity)
        {
            reallocate(new_capacity);
        }
    }

    void resize(size_t new_size, const T& default_val = {})
    {
        if(new_size == m_size)
        {
            return;
        }
        else if(new_size > m_size)
        {
            reallocate_if_needed(new_size - 1);
            for(size_t i = m_size; i < new_size; ++i)
            {
                m_data[i] = default_val;
            }
            m_size = new_size;
        }
        else
        {
            m_size = new_size;
        }
    }

    void pop_back()
    {
        assert(m_size > 0);
        m_size--;
    }

    void push_back(const T& elem)
    {
        reallocate_if_needed(m_size);
        m_data[m_size] = elem;
        ++m_size;
    }

    void push_back(T&& elem)
    {
        reallocate_if_needed(m_size);
        m_data[m_size] = std::move(elem);
        ++m_size;
    }

    T& emplace_back()
    {
        reallocate_if_needed(m_size);
        T& retval = m_data[m_size];
        ++m_size;
        return retval;
    }

    void insert(size_t index, const T& elem)
    {
        assert(index <= m_size);
        reallocate_if_needed(m_size);

        const size_t count = m_size - index;
        for(size_t i = 0; i < count; ++i)
        {
            m_data[m_size - i] = std::move(m_data[m_size - i - 1]);
        }
        m_data[index] = elem;
        ++m_size;
    }

    void insert(size_t index, T&& elem)
    {
        assert(index <= m_size);
        reallocate_if_needed(m_size);
        
        const size_t count = m_size - index;
        for(size_t i = 0; i < count; ++i)
        {
            m_data[m_size - i] = std::move(m_data[m_size - i - 1]);
        }
        m_data[index] = std::move(elem);
        ++m_size;
    }

    void erase(size_t index)
    {
        assert(m_size > 0 && index < m_size);
        --m_size;
        for(index; index < m_size; ++index)
        {
            m_data[index] = std::move(m_data[index + 1]);
        }
    }

    bool operator==(const selftype& other) const
    {
        if(m_size != other.m_size)
        {
            return false;
        }
        else if(this == &other)
        {
            return true;
        }
        for(size_t i = 0; i < m_size; ++i)
        {
            if(m_data[i] != other.m_data[i])
            {
                return false;
            }
        }
        return true;
    }

    selftype& operator=(const selftype& other)
    {
        if(this != &other)
        {
            if(other.m_size > 0)
            {
                reallocate_if_needed(other.m_size - 1);
            }
            for(size_t i = 0; i < other.m_size; ++i)
            {
                m_data[i] = other.m_data[i];
            }
            m_size = other.m_size;
        }
        return *this;
    }

    selftype& operator=(selftype&& other)
    {
        if(this != &other)
        {
            if(m_data)
            {
                delete[] m_data;
            }
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    vector() : m_size(0), m_capacity(0), m_data(nullptr) {}
    vector(const selftype& other) : m_size(0), m_capacity(0), m_data(nullptr) { *this = other; }
    vector(selftype&& other) : m_size(0), m_capacity(0), m_data(nullptr) { *this = std::move(other); }

    ~vector()
    {
        m_size = 0;
        m_capacity = 0;
        if(m_data)
        {
            delete[] m_data;
        }
    }

    inline const T& at(size_t i) const { return m_data[i]; }
    inline T& at(size_t i) { return m_data[i]; }
    inline const T& operator[](size_t i) const { return m_data[i]; }
    inline T& operator[](size_t i) { return m_data[i]; }
    inline const T* data() const { return m_data; }
    inline T* data() { return m_data; }
    inline const T& front() const { return m_data[0]; }
    inline T& front() { return m_data[0]; }
    inline const T& back() const { return m_data[m_size - 1]; }
    inline T& back() { return m_data[m_size - 1]; }
};

}
#pragma once
#include <mutex>
#include <functional>
#include "fixed_vector.h"

namespace ahl::threadsafe
{

template<class T>
class threadsafe_wrapper
{
private:
    std::mutex m_mutex;
    T m_obj;

public:
    threadsafe_wrapper() = default;

    void access(std::function<void(T&)> accessFunc)
    {
        std::scoped_lock lock(m_mutex);
        accessFunc(m_obj);
    }

    void access(std::function<void(const T&)> accessFunc) const
    {
        std::scoped_lock lock(m_mutex);
        accessFunc(m_obj);
    }
};

template <class T>
using fixed_vector = threadsafe_wrapper<ahl::fixed_vector<T>>;

}
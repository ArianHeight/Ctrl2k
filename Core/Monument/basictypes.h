#pragma once
#include <stdint.h>

// null-terminated c-string, please do not use non null-terminated char* buffers if we're specifiying a c_string
typedef const char* c_string;

struct uncopyable
{
    uncopyable() = default;
    uncopyable(const uncopyable& other) = delete;
    uncopyable& operator=(const uncopyable& other) = delete;
};

struct unmoveable
{
    unmoveable(unmoveable&& other) = delete;
    unmoveable& operator=(unmoveable&& other) = delete;
};
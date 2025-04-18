#pragma once
#include <stdint.h>

// null-terminated c-string, please do not use non null-terminated char* buffers if we're specifiying a c_string
typedef const char* c_string;

struct Uncopyable
{
    Uncopyable() = default;
    Uncopyable(const Uncopyable& other) = delete;
    Uncopyable& operator=(const Uncopyable& other) = delete;
};
#pragma once

template<typename T, typename V>
size_t binary_search(const T& arr, size_t len, const V& val)
{
    size_t start = 0;
    size_t end = len;
    size_t i = len >> 1;
    while(start < end)
    {
        if(arr[i] == val)
        {
            return i;
        }

        if(arr[i] < val)
        {
            start = i + 1;
        }
        else
        {
            end = i;
        }
        i = start + ((end - start) >> 1);
    }
    return INVALID_SIZE_T;
}

template<typename T, typename V>
inline size_t binary_search(const T& arr, const V& val)
{
    return binary_search(arr, arr.size(), val);
}

template<typename T, typename V>
size_t binary_search_nearest(const T& arr, size_t len, const V& val)
{
    size_t start = 0;
    size_t end = len;
    size_t i = len >> 1;
    while(start < end)
    {
        const bool smaller = arr[i] < val;
        if(!smaller && (i == 0 || arr[i - 1] < val))
        {
            return i;
        }
        else if(smaller && i == len - 1)
        {
            return len;
        }

        if(smaller)
        {
            start = i + 1;
        }
        else
        {
            end = i;
        }
        i = start + ((end - start) >> 1);
    }
    return INVALID_SIZE_T;
}

template<typename T, typename V>
inline size_t binary_search_nearest(const T& arr, const V& val)
{
    return binary_search_nearest(arr, arr.size(), val);
}

template<typename T, typename V>
size_t linear_search(const T& arr, size_t len, const V& val)
{
    for(size_t i = 0; i < len; ++i)
    {
        if(arr[i] == val)
        {
            return i;
        }
    }
    return INVALID_SIZE_T;
}

template<typename T, typename V>
inline size_t linear_search(const T& arr, const V& val)
{
    return linear_search(arr, arr.size(), val);
}
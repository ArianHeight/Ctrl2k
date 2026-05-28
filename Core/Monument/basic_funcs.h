#pragma once
#include <utility>

consteval inline size_t most_significant_bit(size_t val)
{
    size_t bit = 0;
    while(val >>= 1) { ++bit; }
    return bit;
}

template <typename T, typename V>
size_t binary_search(const T* arr, size_t len, const V& val)
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

template <typename T, typename V>
inline size_t binary_search(const T& arr, const V& val)
{
    return binary_search(arr.data(), arr.size(), val);
}

template <typename T, typename V>
size_t binary_search_nearest(const T* arr, size_t len, const V& val)
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

template <typename T, typename V>
inline size_t binary_search_nearest(const T& arr, const V& val)
{
    return binary_search_nearest(arr.data(), arr.size(), val);
}

template <typename T, typename V>
size_t linear_search(const T* arr, size_t len, const V& val)
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

template <typename T, typename V>
inline size_t linear_search(const T& arr, const V& val)
{
    return linear_search(arr.data(), arr.size(), val);
}

// DONOT CALL THIS
// using std::move degrades performance in debug mode but in release it's just a 0 overhead cast
template <typename T>
inline void _sort_swap(T& v1, T& v2)
{
    T temp = std::move(v1);
    v1 = std::move(v2);
    v2 = std::move(temp);
}

constexpr size_t SORT_AUX_BUFFER_BYTES = 1024; // in bytes

/*

Selection sort

*/

template <typename T>
void selection_sort(T* arr, size_t len)
{
    if(!arr)
        return;

    for(size_t i = 0; i < len; i++)
    {
        size_t smallest_index = i;
        for(size_t j = i; j < len; j++)
        {
            if(arr[j] < arr[smallest_index])
            {
                smallest_index = j;
            }
        }

        _sort_swap(arr[i], arr[smallest_index]);
    }
}

/*

Insertion sort

*/

constexpr size_t SORT_INSERTION_BYTES_THRESHOLD = 128; // in bytes

// we should always use insertion sort under this length(for contiguous data ofc)
// it's actually faster under 12 elements always, but we can do 8 for a nice power of 2
constexpr size_t SORT_INSERTION_MIN_SIZE = 8;

consteval size_t _sort_calc_sort_insertion_size(size_t data_size)
{
    size_t insert_size = (size_t)1 << most_significant_bit(SORT_INSERTION_BYTES_THRESHOLD / data_size);
    return insert_size < SORT_INSERTION_MIN_SIZE ? SORT_INSERTION_MIN_SIZE : insert_size;
}

// using reverse linear search to find the insert location
template <typename T>
void insertion_sort_linear(T* arr, size_t len)
{
    if(!arr)
        return;

    for(size_t i = 1; i < len; i++)
    {
        T val = std::move(arr[i]);

        size_t j;
        for(j = 0; j < i && arr[i - j - 1] > val; j++)
        {
            arr[i - j] = std::move(arr[i - j - 1]);
        }
        arr[i - j] = std::move(val);
    }
}

/*

Quicksort

*/

// DONOT CALL THIS
template <typename T>
inline size_t _quick_sort_median_of_3(T* arr, size_t i1, size_t i2, size_t i3)
{
    if(arr[i1] < arr[i2])
    {
        if(arr[i2] < arr[i3])
        {
            return i2;
        }
        else if(arr[i1] < arr[i3])
        {
            return i3;
        }
        else
        {
            return i1;
        }
    }
    else if(arr[i1] < arr[i3])
    {
        return i1;
    }
    else if(arr[i2] < arr[i3])
    {
        return i3;
    }
    return i2;
}

// DONOT CALL THIS
// maybe we want to add randomness to this?
template <typename T>
inline size_t _quick_sort_select_pivot_index(T* arr, size_t len)
{
    if(len >= 128) // pseudo median of 9
    {
        const size_t m1 = _quick_sort_median_of_3(arr, 0, len / 8, len / 4);
        const size_t m2 = _quick_sort_median_of_3(arr, 3 * len / 8, len / 2, 5 * len / 8);
        const size_t m3 = _quick_sort_median_of_3(arr, 6 * len / 8, 7 * len / 8, len - 1);
        return _quick_sort_median_of_3(arr, m1, m2, m3);
    }
    else if(len >= 16) // median of 3
    {
        return _quick_sort_median_of_3(arr, 0, len >> 1, len - 1);
    }
    return len >> 1; // median index
}

// DONOT CALL THIS
// returns the new pivot index
template <typename T>
FORCE_INLINE size_t _quick_sort_partition(T* arr, size_t len)
{
    T pivot_val = arr[_quick_sort_select_pivot_index(arr, len)]; // this can't be std::move
    size_t left = -1;
    size_t right = len;

    while(true)
    {
        for(++left; arr[left] < pivot_val; ++left);
        for(--right; arr[right] > pivot_val; --right);

        if(left > right)
            break;

        _sort_swap(arr[left], arr[right]);
    }

    return left;
}

// Hoare's partition with dynamic pivot selection method depending on partition size
template <typename T>
void quick_sort_vanilla(T* arr, size_t len)
{
    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(arr[0] > arr[1])
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }

    const size_t pivot_index = _quick_sort_partition(arr, len);

    quick_sort(arr, pivot_index);
    quick_sort(&arr[pivot_index], len - pivot_index);
}

// Hoare's quick sort and adaptive pivot with insertion sort base case
template <typename T>
void quick_sort(T* arr, size_t len)
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));

    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(arr[0] > arr[1])
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }
    else if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort_linear(arr, len);
        return;
    }

    const size_t pivot_index = _quick_sort_partition(arr, len);

    quick_sort(arr, pivot_index);
    quick_sort(&arr[pivot_index], len - pivot_index);
}

/*

Mergesort

*/

// DONOT CALL THIS
template <typename T>
void _merge_sort_merge(const T* arr1, size_t len1, const T* arr2, size_t len2, T* out)
{
    size_t left = 0;
    size_t right = 0;

    size_t index;
    for(index = 0; left < len1 && right < len2; index++)
    {
        if(arr1[left] < arr2[right])
        {
            out[index] = std::move(arr1[left]);
            ++left;
        }
        else
        {
            out[index] = std::move(arr2[right]);
            ++right;
        }
    }

    for(; left < len1; left++, index++)
    {
        out[index] = std::move(arr1[left]);
    }

    for(; right < len2; right++, index++)
    {
        out[index] = std::move(arr2[right]);
    }
}

// DONOT CALL THIS
template <typename T>
FORCE_INLINE void _merge_sort_impl(T* arr, T* buf, size_t start_sort_len, size_t len)
{
    assert(start_sort_len > 0);

    // double buffering
    T* from = arr;
    T* to = buf;

    for(size_t sort_len = start_sort_len; sort_len < len; sort_len = sort_len << 1)
    {
        size_t i;
        for(i = 0; i + (sort_len << 1) < len; i += sort_len << 1)
        {
            _merge_sort_merge(&from[i], sort_len, &from[i + sort_len], sort_len, &to[i]);
        }

        if(i < len)
        {
            if(i + sort_len < len) // we have 1 and a half sub buffers
            {
                _merge_sort_merge(&from[i], sort_len, &from[i + sort_len], len - (i + sort_len), &to[i]);
            }
            else // we have a half sub buffer
            {
                for(; i < len; i++)
                {
                    to[i] = std::move(from[i]);
                }
            }
        }

        // swap buffers
        T* temp = to;
        to = from;
        from = temp;
    }

    // from is where the data is because of the buffer swap
    if(from == buf)
    {
        for(size_t i = 0; i < len; i++)
        {
            arr[i] = std::move(buf[i]);
        }
    }
}

// a double-buffered iterative merge sort
template <typename T>
void merge_sort_vanilla(T* arr, size_t len)
{
    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(arr[0] > arr[1])
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }

    T* buf = new T[len];
    _merge_sort_impl(arr, buf, 1, len);
    delete[] buf;
}

// double buffered iterative merge sort and insertion sort together
template <typename T>
void merge_sort(T* arr, size_t len)
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));

    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(arr[0] > arr[1])
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }
    else if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort_linear(arr, len);
        return;
    }

    size_t i;
    for(i = 0; i + SORT_INSERTION_SIZE < len; i += SORT_INSERTION_SIZE)
    {
        insertion_sort_linear(&arr[i], SORT_INSERTION_SIZE);
    }
    insertion_sort_linear(&arr[i], len - i);

    T* buf = new T[len];
    _merge_sort_impl(arr, buf, SORT_INSERTION_SIZE, len);
    delete[] buf;
}

/*

Ctrlsort

*/

// a composite sort that combines insertion sort, merge sort, and quicksort
template <typename T>
void sort(T* arr, size_t len)
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));
    constexpr size_t SORT_AUX_BUFFER_LEN = SORT_AUX_BUFFER_BYTES / sizeof(T);

    if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort_linear(arr, len);
    }
    else if(len <= SORT_AUX_BUFFER_LEN)
    {
        size_t i;
        for(i = 0; i + SORT_INSERTION_SIZE < len; i += SORT_INSERTION_SIZE)
        {
            insertion_sort_linear(&arr[i], SORT_INSERTION_SIZE);
        }
        insertion_sort_linear(&arr[i], len - i);

        T buf[SORT_AUX_BUFFER_LEN];
        _merge_sort_impl(arr, buf, SORT_INSERTION_SIZE, len);
    }
    else
    {
        const size_t pivot_index = _quick_sort_partition(arr, len);
        sort(arr, pivot_index);
        sort(&arr[pivot_index], len - pivot_index);
    }
}

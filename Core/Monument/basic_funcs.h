#pragma once
#include <utility>

consteval inline size_t most_significant_bit(size_t val)
{
    size_t bit = 0;
    while(val >>= 1) { ++bit; }
    return bit;
}

// DONOT CREATE THIS
template <typename T>
struct _sort_compare_functor
{
    _sort_compare_functor() = default;

    FORCE_INLINE bool operator()(const T& v1, const T& v2) const
    {
        return v1 < v2;
    }
};

template <typename T, class _compare_functor = _sort_compare_functor<T>>
size_t binary_search(const T* arr, size_t len, const T& val, _compare_functor compare = _sort_compare_functor<T>())
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

        if(compare(arr[i], val))
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

template <typename T, class _compare_functor = _sort_compare_functor<T>>
size_t binary_search_position(const T* arr, size_t len, const T& val, _compare_functor compare = _sort_compare_functor<T>())
{
    size_t start = 0;
    size_t end = len;
    size_t i = len >> 1;
    while(start < end)
    {
        const bool smaller = compare(arr[i], val);
        if(!smaller && (i == 0 || compare(arr[i - 1], val)))
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
inline size_t binary_search_position(const T& arr, const V& val)
{
    return binary_search_position(arr.data(), arr.size(), val);
}

template <typename T>
size_t linear_search(const T* arr, size_t len, const T& val)
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
FORCE_INLINE void _sort_swap(T& v1, T& v2)
{
    T temp = std::move(v1);
    v1 = std::move(v2);
    v2 = std::move(temp);
}

constexpr size_t SORT_AUX_BUFFER_BYTES = 1024; // in bytes

/*

Selection sort

*/

template <typename T, class _compare_functor = _sort_compare_functor<T>>
void selection_sort(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    if(!arr)
        return;

    for(size_t i = 0; i < len; i++)
    {
        size_t smallest_index = i;
        for(size_t j = i; j < len; j++)
        {
            if(compare(arr[j], arr[smallest_index]))
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
template <typename T, class _compare_functor>
bool _insertion_sort_attempt(T* arr, size_t len, size_t max_insertions, _compare_functor compare)
{
    if(!arr)
        return false;

    size_t insertions = 0;
    for(size_t i = 1; i < len; i++)
    {
        T val = std::move(arr[i]);

        size_t j;
        for(j = 0; j < i && compare(val, arr[i - j - 1]); j++)
        {
            arr[i - j] = std::move(arr[i - j - 1]);
        }
        arr[i - j] = std::move(val);
        insertions += !(j == 0);

        if(insertions > max_insertions)
        {
            return false;
        }
    }

    return true;
}

// using reverse linear search to find the insert location
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void insertion_sort(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    if(!arr)
        return;

    for(size_t i = 1; i < len; i++)
    {
        T val = std::move(arr[i]);

        size_t j;
        for(j = 0; j < i && compare(val, arr[i - j - 1]); j++)
        {
            arr[i - j] = std::move(arr[i - j - 1]);
        }
        arr[i - j] = std::move(val);
    }
}

/*

Quicksort

*/

constexpr size_t SORT_QUICK_BRANCHLESS_BUF_SIZE = 32;

// DONOT CALL THIS
template <typename T, class _compare_functor>
inline T _quick_sort_median_of_3(const T& v1, const T& v2, const T& v3, _compare_functor compare)
{
    if(compare(v1, v2))
    {
        if(compare(v2, v3))
        {
            return v2;
        }
        else if(compare(v1, v3))
        {
            return v3;
        }
        else
        {
            return v1;
        }
    }
    else if(compare(v1, v3))
    {
        return v1;
    }
    else if(compare(v2, v3))
    {
        return v3;
    }
    return v2;
}

// DONOT CALL THIS
// maybe we want to add randomness to this?
template <typename T, class _compare_functor>
inline T _quick_sort_select_pivot(const T* arr, size_t len, _compare_functor compare)
{
    if(len >= 128) // pseudo median of 9
    {
        return _quick_sort_median_of_3(
            _quick_sort_median_of_3(arr[0], arr[len / 8], arr[len / 4], compare),
            _quick_sort_median_of_3(arr[3 * len / 8], arr[len / 2], arr[5 * len / 8], compare),
            _quick_sort_median_of_3(arr[3 * len / 4], arr[7 * len / 8], arr[len - 1], compare),
            compare);
    }
    else if(len >= 16) // median of 3
    {
        return _quick_sort_median_of_3(arr[0], arr[len >> 1], arr[len - 1], compare);
    }
    return arr[len >> 1]; // median index
}

// DONOT CALL THIS
// returns the new pivot index
template <typename T, class _compare_functor>
FORCE_INLINE size_t _quick_sort_partition(T* arr, size_t len, _compare_functor compare)
{
    const T pivot_val = _quick_sort_select_pivot(arr, len, compare); // this can't be std::move
    size_t left = -1;
    size_t right = len;

    while(true)
    {
        for(++left; compare(arr[left], pivot_val); ++left);
        for(--right; compare(pivot_val, arr[right]); --right);

        if(left > right)
            break;

        _sort_swap(arr[left], arr[right]);
    }

    return left;
}

// DONOT CALL THIS
// returns the new pivot index
template <typename T, class _compare_functor>
FORCE_INLINE size_t _quick_sort_partition_branchless(T* arr, size_t len, size_t& total_swaps, _compare_functor compare)
{
    const T pivot_val = _quick_sort_select_pivot(arr, len, compare); // this can't be std::move
    size_t left = 0;
    size_t right = len - 1;

    size_t swap1[SORT_QUICK_BRANCHLESS_BUF_SIZE];
    size_t swap2[SORT_QUICK_BRANCHLESS_BUF_SIZE];
    size_t num_swap1 = 0;
    size_t num_swap2 = 0;
    size_t start1 = 0;
    size_t start2 = 0;

    while(right - left + 1 > (SORT_QUICK_BRANCHLESS_BUF_SIZE << 1))
    {
        if(num_swap1 == 0)
        {
            start1 = 0;
            for(size_t i = 0; i < SORT_QUICK_BRANCHLESS_BUF_SIZE; i++)
            {
                swap1[num_swap1] = left + i;
                num_swap1 += !compare(arr[left + i], pivot_val);
            }
            left += SORT_QUICK_BRANCHLESS_BUF_SIZE;
        }
        if(num_swap2 == 0)
        {
            start2 = 0;
            for(size_t i = 0; i < SORT_QUICK_BRANCHLESS_BUF_SIZE; i++)
            {
                swap2[num_swap2] = right - i;
                num_swap2 += !compare(pivot_val, arr[right - i]);
            }
            right -= SORT_QUICK_BRANCHLESS_BUF_SIZE;
        }

        const size_t num_swap = num_swap1 < num_swap2 ? num_swap1 : num_swap2;
        for(size_t i = 0; i < num_swap; i++)
        {
            _sort_swap(arr[swap1[start1 + i]], arr[swap2[start2 + i]]);
        }
        num_swap1 -= num_swap;
        num_swap2 -= num_swap;
        start1 += num_swap;
        start2 += num_swap;
        total_swaps += num_swap;
    }

    left = (num_swap1 != 0 ? swap1[start1] : left) - 1;
    right = (num_swap2 != 0 ? swap2[start2] : right) + 1;

    while(true)
    {
        for(++left; compare(arr[left], pivot_val); ++left);
        for(--right; compare(pivot_val, arr[right]); --right);

        if(left > right)
            break;

        _sort_swap(arr[left], arr[right]);
        ++total_swaps;
    }

    return left;
}

// Hoare's partition with dynamic pivot selection method depending on partition size
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void quick_sort_vanilla(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(compare(arr[1], arr[0]))
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }

    const size_t pivot_index = _quick_sort_partition(arr, len, compare);

    quick_sort_vanilla(arr, pivot_index, compare);
    quick_sort_vanilla(&arr[pivot_index], len - pivot_index, compare);
}

// Hoare's quick sort and adaptive pivot with insertion sort base case
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void quick_sort_branched(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));

    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(compare(arr[1], arr[0]))
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }
    else if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort(arr, len, compare);
        return;
    }

    const size_t pivot_index = _quick_sort_partition(arr, len, compare);

    quick_sort_branched(arr, pivot_index, compare);
    quick_sort_branched(&arr[pivot_index], len - pivot_index, compare);
}

// this is block quick sort with Hoare's partition and adaptive pivot with insertion sort base case
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void quick_sort_branchless(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));

    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(compare(arr[1], arr[0]))
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }
    else if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort(arr, len, compare);
        return;
    }

    size_t num_swaps = 0;
    const size_t pivot_index = _quick_sort_partition_branchless(arr, len, num_swaps, compare);

    quick_sort_branchless(arr, pivot_index, compare);
    quick_sort_branchless(&arr[pivot_index], len - pivot_index, compare);
}

// defaults to quick_sort_branchless
template <typename T, class _compare_functor = _sort_compare_functor<T>>
inline void quick_sort(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    quick_sort_branchless(arr, len, compare);
}

/*

Mergesort

*/

// DONOT CALL THIS
template <typename T, class _compare_functor>
void _merge_sort_merge(const T* arr1, size_t len1, const T* arr2, size_t len2, T* out, _compare_functor compare)
{
    size_t left = 0;
    size_t right = 0;

    size_t index;
    for(index = 0; left < len1 && right < len2; index++)
    {
        if(compare(arr1[left], arr2[right]))
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
template <typename T, class _compare_functor>
FORCE_INLINE void _merge_sort_impl(T* arr, T* buf, size_t start_sort_len, size_t len, _compare_functor compare)
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
            _merge_sort_merge(&from[i], sort_len, &from[i + sort_len], sort_len, &to[i], compare);
        }

        if(i < len)
        {
            if(i + sort_len < len) // we have 1 and a half sub buffers
            {
                _merge_sort_merge(&from[i], sort_len, &from[i + sort_len], len - (i + sort_len), &to[i], compare);
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
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void merge_sort_vanilla(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(compare(arr[1], arr[0]))
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }

    T* buf = new T[len];
    _merge_sort_impl(arr, buf, 1, len, compare);
    delete[] buf;
}

// double buffered iterative merge sort and insertion sort together
template <typename T, class _compare_functor = _sort_compare_functor<T>>
void merge_sort(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));

    if(len <= 1 || !arr)
        return;
    else if(len == 2)
    {
        if(compare(arr[1], arr[0]))
        {
            _sort_swap(arr[0], arr[1]);
        }
        return;
    }
    else if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort(arr, len, compare);
        return;
    }

    size_t i;
    for(i = 0; i + SORT_INSERTION_SIZE < len; i += SORT_INSERTION_SIZE)
    {
        insertion_sort(&arr[i], SORT_INSERTION_SIZE, compare);
    }
    insertion_sort(&arr[i], len - i, compare);

    T* buf = new T[len];
    _merge_sort_impl(arr, buf, SORT_INSERTION_SIZE, len, compare);
    delete[] buf;
}

/*

Ctrlsort

*/

constexpr float SORT_ATTEMPT_INSERTION_PERCENTAGE = 0.1f;
compile_assert(SORT_ATTEMPT_INSERTION_PERCENTAGE >= 0.f && SORT_ATTEMPT_INSERTION_PERCENTAGE < 0.5f);

// DONOT CALL THIS
template <typename T, class _compare_functor>
void _sort_impl(T* arr, size_t len, size_t num_prev_swaps, _compare_functor compare)
{
    constexpr size_t SORT_INSERTION_SIZE = _sort_calc_sort_insertion_size(sizeof(T));
    constexpr size_t SORT_AUX_BUFFER_LEN = SORT_AUX_BUFFER_BYTES / sizeof(T);

    if(len <= SORT_INSERTION_SIZE)
    {
        insertion_sort(arr, len, compare);
    }
    else if(len <= SORT_AUX_BUFFER_LEN)
    {
        size_t i;
        for(i = 0; i + SORT_INSERTION_SIZE < len; i += SORT_INSERTION_SIZE)
        {
            insertion_sort(&arr[i], SORT_INSERTION_SIZE, compare);
        }
        insertion_sort(&arr[i], len - i, compare);

        T buf[SORT_AUX_BUFFER_LEN];
        _merge_sort_impl(arr, buf, SORT_INSERTION_SIZE, len, compare);
    }
    else
    {
        if(num_prev_swaps < len)
        {
            const size_t try_insertions = num_prev_swaps < (len >> 1) ? num_prev_swaps : len - num_prev_swaps;
            if(try_insertions < (size_t)(SORT_ATTEMPT_INSERTION_PERCENTAGE * len))
            {
                if(_insertion_sort_attempt(arr, len, try_insertions, compare))
                {
                    return;
                }
            }
        }

        size_t num_swaps = 0;
        const size_t pivot_index = _quick_sort_partition_branchless(arr, len, num_swaps, compare);

        _sort_impl(arr, pivot_index, num_swaps, compare);
        _sort_impl(&arr[pivot_index], len - pivot_index, num_swaps, compare);
    }
}

// a composite sort that combines insertion sort, merge sort, and quicksort(with optimistic insertion sort if there was not alot of swapping)
// currently this is somewhat similar to pdqsort, instead without the partitionLeft and partitionRight functionality for equal values
template <typename T, class _compare_functor = _sort_compare_functor<T>>
inline void sort(T* arr, size_t len, _compare_functor compare = _sort_compare_functor<T>())
{
    _sort_impl(arr, len, INVALID_SIZE_T, compare);
}

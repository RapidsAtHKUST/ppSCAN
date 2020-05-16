#pragma once

#include <x86intrin.h>

#include <cstdint>

#include "util/log/log.h"

template<typename T>
uint32_t LinearSearch(T *array, uint32_t offset_beg, uint32_t offset_end, T val) {
    // linear search fallback
    for (auto offset = offset_beg; offset < offset_end; offset++) {
        if (array[offset] >= val) {
            return offset;
        }
    }
    return offset_end;
}

template<typename T>
uint32_t BranchFreeBinarySearch(T *a, uint32_t offset_beg, uint32_t offset_end, T x) {
    int32_t n = offset_end - offset_beg;
    using I = uint32_t;
    const T *base = a + offset_beg;
    while (n > 1) {
        I half = n / 2;
        __builtin_prefetch(base + half / 2, 0, 0);
        __builtin_prefetch(base + half + half / 2, 0, 0);
        base = (base[half] < x) ? base + half : base;
        n -= half;
    }
    return (*base < x) + base - a;
}

// require sizeof(T) to be 4
template<typename T>
uint32_t BinarySearchForGallopingSearch(const T *array, uint32_t offset_beg, uint32_t offset_end, T val) {
    while (offset_end - offset_beg >= 32) {
        auto mid = static_cast<uint32_t>((static_cast<unsigned long>(offset_beg) + offset_end) / 2);
        _mm_prefetch((char *) &array[(static_cast<unsigned long>(mid + 1) + offset_end) / 2], _MM_HINT_T0);
        _mm_prefetch((char *) &array[(static_cast<unsigned long>(offset_beg) + mid) / 2], _MM_HINT_T0);
        if (array[mid] == val) {
            return mid;
        } else if (array[mid] < val) {
            offset_beg = mid + 1;
        } else {
            offset_end = mid;
        }
    }

    // linear search fallback
    for (auto offset = offset_beg; offset < offset_end; offset++) {
        if (array[offset] >= val) {
            return offset;
        }
    }
    return offset_end;
}

template<typename T>
bool BranchFreeBSExists(T *a, uint32_t offset_beg, uint32_t offset_end, T x) {
    if (offset_beg == offset_end)return false;
    auto it = BranchFreeBinarySearch(a, offset_beg, offset_end, x);
    return it != offset_end && a[it] == x;
}

// Assuming (offset_beg != offset_end)
template<typename T>
uint32_t GallopingSearch(T *array, uint32_t offset_beg, uint32_t offset_end, T val) {
    if (array[offset_end - 1] < val) {
        return offset_end;
    }
    // galloping
    if (array[offset_beg] >= val) {
        return offset_beg;
    }
    if (array[offset_beg + 1] >= val) {
        return offset_beg + 1;
    }
    if (array[offset_beg + 2] >= val) {
        return offset_beg + 2;
    }

    auto jump_idx = 4u;
    while (true) {
        auto peek_idx = offset_beg + jump_idx;
        if (peek_idx >= offset_end) {
            return BranchFreeBinarySearch(array, (jump_idx >> 1u) + offset_beg + 1, offset_end, val);
        }
        if (array[peek_idx] < val) {
            jump_idx <<= 1u;
        } else {
            return array[peek_idx] == val ? peek_idx :
                   BranchFreeBinarySearch(array, (jump_idx >> 1u) + offset_beg + 1, peek_idx + 1, val);
        }
    }
}

#ifdef __AVX2__

uint32_t LinearSearchAVX2(int *array, uint32_t offset_beg, uint32_t offset_end, int val);

uint32_t GallopingSearchAVX2(int *array, uint32_t offset_beg, uint32_t offset_end, int val);

#endif

#ifdef __AVX512F__

uint32_t LinearSearchAVX512(int *array, uint32_t offset_beg, uint32_t offset_end, int val);


uint32_t GallopingSearchAVX512(int *array, uint32_t offset_beg, uint32_t offset_end, int val);

#endif
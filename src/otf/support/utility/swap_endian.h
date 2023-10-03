#pragma once

#include <algorithm>
#include <cstdint>
#include <array>

template <typename T>
void swap_endian( T &val ) {
    union U {
        T val;
        std::array< std::uint8_t, sizeof(T) > raw;
    } src, dst;

    src.val = val;
    std::reverse_copy( src.raw.begin(), src.raw.end(), dst.raw.begin() );
    val = dst.val;
}

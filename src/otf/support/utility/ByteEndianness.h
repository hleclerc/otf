#pragma once

#include <endian.h>

enum class ByteEndianness {
    little, // intel, ...
    big, //

    #if __BYTE_ORDER == __LITTLE_ENDIAN
        host = little
    #else
        host = big
    #endif
};

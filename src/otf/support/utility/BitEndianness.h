#pragma once

#include "../types/TypeConfig.h"

enum class BitEndianness {
    little, // bit 0 = 2^0
    big,    // bit 0 = 2^7
};

// to get bit number `ind`
inline PI8 msk_ind( PI32 ind, BitEndianness eds ) {
    return eds == BitEndianness::little ? PI8( 0x01 ) << ind : PI8( 0x80 ) >> ind;
}

// from bit number `beg` (included) to the last one
inline PI8 msk_beg( PI32 beg, BitEndianness eds ) {
    return eds == BitEndianness::little ? ( 0xFF << beg ) : ( 0xFF >> beg );
}

// from first bit to bit number `end` (excluded)
inline PI8 msk_end( PI32 end, BitEndianness eds ) {
    return eds == BitEndianness::little ? ~( 0xFF << end ) : ~( 0xFF >> end );
}

// from bit number `beg` (included) to bit number `end` (excluded)
inline PI8 msk_mid( PI32 beg, PI32 end, BitEndianness eds ) {
    return eds == BitEndianness::little ? ( 0xFF << beg ) & ~( 0xFF << end ) : ( 0xFF >> beg ) & ~( 0xFF >> end );
}

#include "DispOstWriteContext.h"

void DispOstWriteContext::inc_level() {
    nsp += 4;
}

void DispOstWriteContext::dec_level() {
    nsp -= 4;
}

void DispOstWriteContext::write_sep() {
    *os << '\n';
    for( int i = 0; i < nsp; ++i )
        *os << ' ';
}

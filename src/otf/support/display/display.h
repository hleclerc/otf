#pragma once

#include "Displayer.h"
#include <memory>

// numbers
#define  BASE_TYPE( NAME, ... ) inline void display( Displayer &sr, NAME value ) { sr.add_number( value ); }
#include "../types/base_arithmetic_types.h"
#undef BASE_TYPE

inline void display( Displayer &sr, Byte value ) { sr.add_number( value ); }

// string
inline void display( Displayer &sr, const String &value ) { sr.add_string( value ); }
inline void display( Displayer &sr, const char *value ) { sr.add_string( value ); }
inline void display( Displayer &sr, StringView value ) { sr.add_string( value ); }
inline void display( Displayer &sr, char value ) { sr.add_string( { &value, 1 } ); }

// ptrs
TCT void display( Displayer &sr, const std::unique_ptr<T> &value ) { sr.add_ptr( value.get() ); }
TCT void display( Displayer &sr, const T *value ) { sr.add_ptr( value ); }
TCT void display( Displayer &sr, T *value ) { sr.add_ptr( value ); }

//
TCAB void display( Displayer &sr, const std::pair<A,B> &value ) { sr.beg_array(); sr.add_value( value.first ); sr.add_value( value.second ); sr.end_array(); }

// generic version
TCT void display( Displayer &sr, const T &value ) {
    // array (with begin/end)
    if constexpr( requires { value.begin(); } ) {
        sr.beg_array();
        for( const auto &v : value )
            sr.add_value( v );
        sr.end_array();
    }
    // obj.serialize( sr )
    else {
        return value.display( sr );
    }
}


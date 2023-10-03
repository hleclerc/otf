#pragma once

#include "../containers/Vec.h"
#include "../utility/ASSERT.h"
#include "to_string.h"

TCU String join( const U &vec, String sep = ", " ) {
    String res;
    for( const auto &v : vec ) {
        if ( res.size() )
            res += sep;
        res += to_string( v );
    }
    return res;
}

//inline String join( const Vec<String> &vec, String sep = ", " ) {
//    String res;
//    for( ST i = 0; i < vec.size(); ++i ) {
//        if ( i )
//            res += sep;
//        res += vec[ i ];
//    }
//    return res;
//}

inline String join_binary( const Vec<String> &vec, String func = "std::max" ) {
    ASSERT( vec.size() );

    String res = vec[ 0 ];
    for( PI i = 1; i < vec.size(); ++i )
        res = func + "( " + res + ", " + vec[ i ] + " )";
    return res;
}

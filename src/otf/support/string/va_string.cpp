#include "../utility/ERROR.h"
#include "va_string.h"

String va_string_repl_vec( const Vec<String> &values ) {
    auto is_a_letter = []( char c ) { return ( c >= 'A' && c <= 'Z' ) || c == '_'; };
    auto is_a_number = []( char c ) { return c >= '0' && c <= '9'; };

    if ( values.empty() )
        return {};

    Vec<PI> map_inds, num_inds;
    for( PI n = 1; n < values.size(); ++n ) {
        if ( values[ n ].ends_with( ":" ) )
            map_inds << n++;
        else
            num_inds << n;
    }

    auto find_key = [&]( String str ) -> const String * {
        for( PI i = 0; i < map_inds.size(); ++i )
            if ( values[ map_inds[ i ] ].substr( 0, values[ map_inds[ i ] ].size() - 1 ) == str )
                return &values[ map_inds[ i ] + 1 ];
        return nullptr;
    };

    String res;
    const String &str = values[ 0 ];
    res.reserve( 2 * str.size() );
    for( PI i = 0; i < str.size(); ++i ) {
        if ( str[ i ] == '{' ) {
            // number
            if ( i + 1 < str.size() && is_a_number( str[ i + 1 ] ) ) {
                PI j = i, v = 0;
                while( ++j < str.size() && is_a_number( str[ j ] ) )
                    v = 10 * v + ( str[ j ] - '0' );
                if ( j < str.size() && str[ j ] == '}' ) {
                    if ( v >= num_inds.size() ) {
                        std::cerr << "bad index in va_string " << str << std::endl;
                        assert( 0 );
                    }
                    res += values[ num_inds[ v ] ];
                    i = j;
                    continue;
                }
            }

            // variable
            if ( i + 1 < str.size() && is_a_letter( str[ i + 1 ] ) ) {
                PI j = i;
                while( ++j < str.size() && is_a_letter( str[ j ] ) )
                    ;
                if ( j < str.size() && str[ j ] == '}' ) {
                    String key = str.substr( i + 1, j - i - 1 );
                    const String *val = find_key( key );
                    if ( ! val ) {
                        std::cerr << "no key '" << key << "' in va_string " << str << std::endl;
                        assert( 0 );
                    }
                    res += *val;
                    i = j;
                    continue;
                }
            }
        }

        res += str[ i ];
    }

    return res;
}

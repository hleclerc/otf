#include "Displayer.h"

static bool is_space( char c ) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

static StringView strip( StringView str ) {
    while ( str.size() && is_space( str.front() ) ) str.remove_prefix( 1 );
    while ( str.size() && is_space( str.back () ) ) str.remove_suffix( 1 );
    return str;
};

Displayer::~Displayer() {
}

void Displayer::add_number( Byte value ) {
    return add_number( PI8( value ) );
}

StringView Displayer::read_arg_name( StringView &arg_names ) {
    int nb_open_par = 0;
    for( PI i = 0; i < arg_names.size(); ++i ) {
        // "..."
        if ( arg_names[ i ] == '"' ) {
            while ( true ) {
                if ( ++i == arg_names.size() ) {
                    --i;
                    break;
                }
                if ( arg_names[ i ] == '\\' && i + 1 < arg_names.size() ) {
                    // TODO: multi char escape sequence
                    ++i;
                    continue;
                }
                if ( arg_names[ i ] == '"' )
                    break;
            }
            continue;
        }

         if ( arg_names[ i ] == '(' || arg_names[ i ] == '[' || arg_names[ i ] == '{' ) {
             ++nb_open_par;
             continue;
         }

         if ( arg_names[ i ] == ')' || arg_names[ i ] == ']' || arg_names[ i ] == '}' ) {
             --nb_open_par;
             continue;
         }

        if ( arg_names[ i ] == ',' && nb_open_par == 0 ) {
            StringView res = arg_names.substr( 0, i );
            arg_names.remove_prefix( i + 1 );
            return strip( res );
        }
    }

    return strip( arg_names );
}

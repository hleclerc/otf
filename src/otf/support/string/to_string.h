#pragma once

#include "../display/Displayer_Ostream/Displayer_Ostream.h"
#include <sstream>

TCT std::string to_string( T &&val, Displayer_Ostream &&ds = {} ) {
    ds.add_value( std::forward<T>( val ) );
    std::ostringstream ss;
    ds.write( ss );

    return ss.str();
}

#pragma once

#include "../containers/Opt.h"
#include <sstream>
#include <fstream>
#include <string>

inline Opt<std::string> read_file( const StringView &filename ) {
    std::ifstream fi( std::string{ filename } );
    if ( ! fi )
        return {};
    std::stringstream is;
    is << fi.rdbuf();
    return is.str();
}

#pragma once

#include "../TypeConfig.h"

inline String strip( StringView res ) {
    while ( res.size() && res.back() == ' ' )
        res.remove_suffix( 1 );
    while ( res.size() && res.front() == ' ' )
        res.remove_prefix( 1 );

    return String{ res };
};

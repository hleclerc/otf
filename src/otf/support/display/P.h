#pragma once

#include "Displayer_Ostream/Displayer_Ostream.h"

template<class OS,class... ArgValues>
void __print_with_mutex( Displayer_Ostream &&ds, OS &os, StringView arg_names, ArgValues &&...arg_values ) {
    // make the tree
    ( ds.add_attribute( ds.read_arg_name( arg_names ), std::forward<ArgValues>( arg_values ) ), ... );

    // write
    static std::mutex m;
    m.lock();
    ds.write( os );
    os << std::endl;
    m.unlock();
}

template<class... ArgValues>
void __show( Displayer_Ostream &&ds, StringView arg_names, ArgValues &&...arg_values ) {
    // make the tree
    ( ds.add_attribute( ds.read_arg_name( arg_names ), std::forward<ArgValues>( arg_values ) ), ... );

    // show it
    ds.show();
}

#ifndef P
    // PRINT in cout
    #define P( ... ) \
        __print_with_mutex( Displayer_Ostream(), std::cout, #__VA_ARGS__, __VA_ARGS__ )

    // PRINT in cout with options
    #define PO( VALUE, PARAMS ) \
        __print_with_mutex( std::cout, " -> ", ", ", PARAMS, #VALUE, VALUE )

    // PRINT with .display in cout with options
    #define PD( VALUE, ... ) \
        ( VALUE ).display( __VA_ARGS__ ).display( std::cout  << #VALUE " -> \n" ) << std::endl

    // PRINT in cerr
    #define PE( ... ) \
        __print_with_mutex( Displayer_Ostream(), std::cerr, #__VA_ARGS__, __VA_ARGS__ )

    // PRINT with file and line info
    #define PM( ... ) \
        __print_with_mutex( std::cout << __FILE__ << ':' << __LINE__, " -> ", ", ", {}, #__VA_ARGS__, __VA_ARGS__, WithSep{""},  )

    // Display graph
    #define SHOW( ... ) \
        __show( Displayer_Ostream(), #__VA_ARGS__, __VA_ARGS__ )

    // PRINT counter
    #define PC do { static int cpt = 0; PE( cpt++ ); } while ( false )
#endif

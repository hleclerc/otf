#pragma once

#include <assert.h>

#ifdef KSDOT_DEBUG
    #define ASSERT_IF_DEBUG( COND ) assert( COND );
#else
    #define ASSERT_IF_DEBUG( COND )
#endif

#define ASSERTED( VALUE ) ( [&]( auto &&v ) { assert( v ); return v; } )( VALUE )
#define ASSERT( COND ) ( [&]( bool v ) { if ( v ) return; std::cerr << __FILE__ << ":" << __LINE__ << ": condition not met: " #COND ";"; assert( 0 ); } )( COND )

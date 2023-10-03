#pragma once

#include "../types/TypeConfig.h"
#include <cstdlib>

/**
 * static version (known nb items)
*/
template<class T,int rese_in_bytes,int alig_in_bytes>
class DataPtr {
public:
    /**/           DataPtr      ( T *ptr = nullptr ) : ptr( ptr ) {}

    static DataPtr allocate () { if ( rese_in_bytes == 0 ) return {}; return DataPtr{ reinterpret_cast<T *>( std::malloc( rese_in_bytes ) ), rese_in_bytes }; }
    void           free     () { if ( ptr ) std::free( ptr ); }

    const auto&    operator*() const { return *ptr; }
    auto &         operator*() { return *ptr; }

    T*             ptr;     ///<
};

/**
 * dynamic version
*/
template<class T,int alig_in_bytes>
class DataPtr<T,-1,alig_in_bytes> {
public:
    /**/           DataPtr  ( T *ptr, PI rese ) : ptr( ptr ), rese( rese ) {}
    /**/           DataPtr  () : ptr( nullptr ), rese( 0 ) {}

    static DataPtr allocate ( PI nb_items ) { if ( nb_items == 0 ) return {}; PI rese = sizeof( T ) * nb_items; return DataPtr{ reinterpret_cast<T *>( std::malloc( rese ) ), rese }; }
    void           free     () { if ( rese ) std::free( ptr ); }

    const auto&    operator*() const { return *ptr; }
    auto &         operator*() { return *ptr; }

    T*             ptr;     ///<
    PI             rese;    ///< in bytes
};

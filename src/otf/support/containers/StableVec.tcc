#pragma once

#include "StableVec.h"

template<class T,int s>
StableVec<T,s>::StableVec() : _size( 0 ) {
}

template<class T,int s>
StableVec<T,s>::~StableVec() {
    if ( PI j = _buckets.size() ) {
        // last bucket
        char *b = _buckets[ --j ];
        for( PI i = _size % s; --i; )
            reinterpret_cast<T *>( b + i * sizeof( T ) )->~T();
        free( b );

        // previous (full) ones
        while( j-- ) {
            char *b = _buckets[ j ];
            for( PI i = s; --i; )
                reinterpret_cast<T *>( b + i * sizeof( T ) )->~T();
            free( b );
        }
    }
}

template<class T,int s>
const T &StableVec<T,s>::operator[]( PI index ) const {
    return reinterpret_cast<const T *>( _buckets[ index / s ] )[ index % s ];
}

template<class T,int s>
T &StableVec<T,s>::operator[]( PI index ) {
    return reinterpret_cast<T *>( _buckets[ index / s ] )[ index % s ];
}

template<class T,int s>
PI StableVec<T,s>::size() const {
    return _size;
}

template<class T,int s>
TCA T &StableVec<T,s>::secure_get( PI index, const A &...default_value ) {
    // intermediate buckets
    PI num_bucket = index / s;
    while ( num_bucket > _buckets.size() )
        _add_bucket( s, default_value... );

    // last one
    if ( num_bucket >= _buckets.size() ) {
        _add_bucket( index % s + 1, default_value... );
        _size = index + 1;
    } else {
        char *bucket = _buckets[ num_bucket ];
        while ( index >= _size )
            new ( bucket + ( _size++ ) * sizeof( T ) ) T( default_value... );
    }

    // ref
    return reinterpret_cast<T *>( _buckets[ num_bucket ] )[ index % s ];
}

template<class T,int s>
TCA T *StableVec<T,s>::push_back( A &&...args ) {
    T *res;

    if ( _size % s ) {
        res = new ( _buckets[ _size / s ] + _size * sizeof( T ) ) T( std::forward<A>( args )... );
    } else {
        char *new_bucket = reinterpret_cast<char *>( malloc( s * sizeof( T ) ) );
        res = new ( new_bucket ) T( std::forward<A>( args )... );
        _buckets << new_bucket;
    }

    ++_size;

    return res;
}

template<class T,int s>
TCA void StableVec<T,s>::_add_bucket( PI size, const A &...default_value ) {
    char *new_bucket = reinterpret_cast<char *>( malloc( s * sizeof( T ) ) );
    for( PI i = 0; i < size; ++i )
        new ( new_bucket + i * sizeof( T ) ) T( default_value... );
    _buckets << new_bucket;
}

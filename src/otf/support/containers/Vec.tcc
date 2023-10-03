#pragma once

#include "Vec.h"

template<class T>
TCU Vec<T,-1,-1> Vec<T,-1,-1>::from_iterable( const U &iterable, PI size ) {
    TODO;
}

template<class T>
TCU Vec<T,-1,-1> Vec<T,-1,-1>::from_iterable( const U &iterable ) {
    Vec res;
    res._init_from( iterable );
    return res;
}

template<class T>
TCU Vec<T,-1,-1> Vec<T,-1,-1>::from_function( PI size, U &&func ) {
    return { Function{}, size, std::forward<U>( func ) };
}

template<class T>
Vec<T,-1,-1> Vec<T,-1,-1>::from_reservation( PI rese ) {
    Vec res;
    res._init_rese( rese );
    return res;
}

template<class T>
TCA Vec<T,-1,-1> Vec<T,-1,-1>::from_values( A &&...values ) {
    Vec res = Vec::from_reservation( sizeof...( A ) );
    ( res.push_back_in_reserved( std::forward<A>( values ) ), ... );
    return res;
}

template<class T>
TCA Vec<T,-1,-1> Vec<T,-1,-1>::from_size( PI size, A &&...args ) {
    Vec res = from_reservation( size );
    for( PI i = 0; i < size; ++i )
        res.push_back( std::forward<A>( args )... );
    return res;
}

template<class T>
Vec<T,-1,-1> Vec<T,-1,-1>::from_size( PI size ) {
    Vec res;
    res._init_size( size );
    return res;
}

template<class T>
TCU Vec<T,-1,-1>::Vec( std::initializer_list<U> &&that ) {
    _init_from( std::move( that ) );
}

template<class T>
TCU Vec<T,-1,-1>::Vec( Function, PI size, U &&func ) {
    _init_rese( size );
    for( PI i = 0; i < size; ++i )
        new ( _data.ptr + i ) T( func( i ) );
    _size = size;
}

template<class T>
Vec<T,-1,-1>::Vec( const Vec &that ) {
    _init_from( that );
}

template<class T>
Vec<T,-1,-1>::Vec( Vec &&that ) : _data( std::exchange( that._data, {} ) ), _size( std::exchange( that._size, 0 ) ) {
}

template<class T>
TCU Vec<T,-1,-1>::Vec( U &&that ) {
    _init_from( std::forward<U>( that ) );
}

template<class T>
Vec<T,-1,-1>::Vec() : _size( 0 ) {
}

template<class T>
Vec<T,-1,-1>::~Vec() {
    _delete( _data, _size );
}

template<class T>
Vec<T,-1,-1> &Vec<T,-1,-1>::operator=( const Vec &that ) {
    auto old_data = _data;
    auto old_size = _size;
    _init_from( that );
    _delete( old_data, old_size );
    return *this;
}


template<class T>
Vec<T,-1,-1> &Vec<T,-1,-1>::operator=( Vec &&that ) {
    _delete( _data, _size );
    _data = std::exchange( that._data, {} );
    _size = std::exchange( that._size, 0 );
    return *this;
}

template<class T>
TCU Vec<T,-1,-1> &Vec<T,-1,-1>::operator=( U &&that ) {
    auto old_data = _data;
    auto old_size = _size;
    _init_from( std::forward<U>( that ) );
    _delete( old_data, old_size );
    return *this;
}

template<class T>
const T &Vec<T,-1,-1>::at( PI index ) const {
    ASSERT_IF_DEBUG( index < size() );
    return _data.ptr[ index ];
}

template<class T>
T &Vec<T,-1,-1>::at( PI index ) {
    ASSERT_IF_DEBUG( index < size() );
    return _data.ptr[ index ];
}

template<class T>
void Vec<T,-1,-1>::_init_rese( PI nb_items ) {
    _data = _allocate( nb_items );
    _size = 0;
}

template<class T>
void Vec<T,-1,-1>::_init_size( PI nb_items ) {
    _data = _allocate( nb_items );
    _size = nb_items;
    for( PI i = 0; i < nb_items; ++i )
        new ( _data.ptr + i ) T;
}

template<class T>
typename Vec<T,-1,-1>::Data Vec<T,-1,-1>::_allocate( PI nb_items ) {
    return Data::allocate( nb_items );
}

template<class T>
void Vec<T,-1,-1>::_delete( Data data, PI size ) {
    while ( size-- )
        data.ptr[ size ].~T();
    _free( data );
}

template<class T>
void Vec<T,-1,-1>::_free( Data data ) {
    data.free();
}

template<class T>
bool Vec<T,-1,-1>::empty() const {
    return _size == 0;
}

template<class T>
PI Vec<T,-1,-1>::size() const {
    return _size;
}

template<class T>
PI Vec<T,-1,-1>::reserved_bytes() const {
    return _data.rese;
}

template<class T>
PI Vec<T,-1,-1>::reserved() const {
    return _data.rese / sizeof( T );
}

template<class T>
typename Vec<T,-1,-1>::Data Vec<T,-1,-1>::moved_data() {
    _size = 0;
    return std::exchange( _data, {} );
}

template<class T>
Vec<T,-1,-1>& Vec<T,-1,-1>::operator<<( const T &val ) {
    push_back( val );
    return *this;
}

template<class T>
Vec<T,-1,-1>& Vec<T,-1,-1>::operator<<( T &&val ) {
    push_back( std::move( val ) );
    return *this;
}

template<class T>
TCA T *Vec<T,-1,-1>::push_back_in_reserved( A &&...args ) {
    return new ( _data.ptr + _size++ ) T( std::forward<A>( args )... );
}

template<class T>
TCA T *Vec<T,-1,-1>::push_back( A &&...args ) {
    if ( ( _size + 1 ) * sizeof( T ) >= _data.rese )
        reserve( _size ? 2 * _size : 2 );
    return push_back_in_reserved( std::forward<A>( args )... );
}

template<class T>
void Vec<T,-1,-1>::reserve( PI rese ) {
    if ( rese * sizeof( T ) > _data.rese ) {
        auto old_data = std::exchange( _data, _allocate( rese ) );
        for( PI i = 0; i < _size; ++i )
            new( _data.ptr + i ) T( std::move( old_data.ptr[ i ] ) );
        _delete( old_data, _size );
    }
}

template<class T>
TCA void Vec<T,-1,-1>::resize( PI size, A &&...args ) {
    reserve( size );
    while( _size > size )
        _data.ptr[ --_size ].~T();
    for( ; _size < size; ++_size )
        new ( _data.ptr + _size ) T( std::forward<A>( args )... );
}

template<class T>
TCU auto Vec<T,-1,-1>::_init_from( U &&that ) -> decltype( std::begin( that ), std::end( that ), std::size( that ), void() ) {
    PI s = std::size( that );
    _init_rese( s );
    _size = s;
    PI c = 0;
    if constexpr ( std::is_rvalue_reference_v<U> )
        for( auto &v : that )
            new ( _data.ptr + c++ ) T( std::move( v ) );
    else
        for( const auto &v : that )
            new ( _data.ptr + c++ ) T( v );
}

template<class T> TCU
T *Vec<T,-1,-1>::insert( PI pos, U &&val ) {
    if ( pos == size() )
        return push_back( val );
    // TODO: reserver without copy
    reserve( _size + 1 );
    new ( _data.ptr + _size ) T( std::move( _data.ptr[ _size - 1 ] ) );
    for( PI i = _size - 1; i > pos; --i )
        _data.ptr[ i ] = std::move( _data.ptr[ i - 1 ] );
    _data.ptr[ pos ] = std::forward<U>( val );
    ++_size;
    return _data.ptr + pos;
}

template<class T>
void Vec<T,-1,-1>::pop_back() {
    _data.ptr[ --_size ].~T();
}

// -----------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------- Static Vec -------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
template<class T,int s>
TCU Vec<T,s,-1> Vec<T,s,-1>::from_function( U &&func ) {
    return { Function{}, std::forward<U>( func ) };
}

template<class T,int s>
Vec<T,s,-1> Vec<T,s,-1>::zeroes() {
    return { Fill{}, 0 };
}

template<class T,int s>
TCU Vec<T,s,-1>::Vec( std::initializer_list<U> &&that ) {
    ASSERT_IF_DEBUG( s == std::size( that ) );
    int c = 0;
    for( auto &v : that )
        new ( ptr() + c++ ) T( std::move( v ) );
}

template<class T,int s>
TCA Vec<T,s,-1>::Vec( Fill, A&&...args ) {
    for( int i = 0; i < s; ++i )
        new ( ptr() + i ) T( std::forward<A>( args )... );
}

template<class T,int s>
TCU Vec<T,s,-1>::Vec( Function, U &&func ) {
    for( int i = 0; i < s; ++i )
        new ( ptr() + i ) T( func( i ) );
}

template<class T,int s>
Vec<T,s,-1>::Vec( const Vec &that ) {
    for( int i = 0; i < s; ++i )
        new ( ptr() + i ) T( that[ i ] );
}

template<class T,int s>
Vec<T,s,-1>::Vec( Vec &&that ) {
    for( int i = 0; i < s; ++i )
        new ( ptr() + i ) T( std::move( that[ i ] ) );
}

template<class T,int s>
TCU Vec<T,s,-1>::Vec( const U &that ) {
    ASSERT_IF_DEBUG( s == std::size( that ) );
    int c = 0;
    for( const auto &v : that )
        new ( ptr() + c++ ) T( v );
}

template<class T,int s>
TCU Vec<T,s,-1>::Vec( U &&that ) {
    ASSERT_IF_DEBUG( s == std::size( that ) );
    int c = 0;
    for( auto &v : that )
        new ( ptr() + c++ ) T( std::move( v ) );
}

template<class T,int s>
Vec<T,s,-1>::Vec() {
    for( int i = 0; i < s; ++i )
        new ( ptr() + i ) T;
}

template<class T,int s>
Vec<T,s,-1>::~Vec() {
    for( int i = s; i--; )
        ( ptr() + i )->~T();
}

template<class T,int s>
Vec<T,s,-1> &Vec<T,s,-1>::operator=( const Vec &that ) {
    for( int i = 0; i < s; ++i )
        *( ptr() + i ) = that[ i ];
    return *this;
}


template<class T,int s>
Vec<T,s,-1> &Vec<T,s,-1>::operator=( Vec &&that ) {
    for( int i = 0; i < s; ++i )
        *( ptr() + i ) = std::move( that[ i ] );
    return *this;
}

template<class T,int s>
TCU Vec<T,s,-1> &Vec<T,s,-1>::operator=( const U &that ) {
    ASSERT_IF_DEBUG( s == std::size( that ) );
    int c = 0;
    for( const auto &v : that )
        at( c++ ) = v;
    return *this;
}

template<class T,int s>
TCU Vec<T,s,-1> &Vec<T,s,-1>::operator=( U &&that ) {
    ASSERT_IF_DEBUG( s == std::size( that ) );
    int c = 0;
    for( auto &v : that )
        at( c++ ) = std::move( v );
    return *this;
}

template<class T,int s>
const T &Vec<T,s,-1>::at( PI index ) const {
    ASSERT_IF_DEBUG( index < size() );
    return ptr()[ index ];
}

template<class T,int s>
T &Vec<T,s,-1>::at( PI index ) {
    ASSERT_IF_DEBUG( index < size() );
    return ptr()[ index ];
}

template<class T,int s>
bool Vec<T,s,-1>::empty() const {
    return s == 0;
}

template<class T,int s>
PI Vec<T,s,-1>::size() const {
    return s;
}

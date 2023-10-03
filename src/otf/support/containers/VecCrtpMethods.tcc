#pragma once

#include "VecCrtpMethods.h"

template<class Ch> const Ch& VecCrtpMethods<Ch>::cast() const {
    return static_cast<const Ch &>( *this );
}

template<class Ch> Ch& VecCrtpMethods<Ch>::cast() {
    return static_cast<Ch &>( *this );
}

template<class Ch> const auto& VecCrtpMethods<Ch>::operator[]( PI index ) const {
    return cast().at( index );
}

template<class Ch> auto& VecCrtpMethods<Ch>::operator[]( PI index ) {
    return cast().at( index );
}

template<class Ch> TCAB auto VecCrtpMethods<Ch>::reduction( const A &self_func, B init ) const {
    for( const auto &v : cast() )
        self_func( init, v );
    return init;
}

template<class Ch> auto VecCrtpMethods<Ch>::product() const {
    return reduction( []( auto &v, auto n ) { v *= n; }, typename Ch::T( 1 ) );
}

template<class Ch> auto VecCrtpMethods<Ch>::sum() const {
    return reduction( []( auto &v, auto n ) { v += n; }, typename Ch::T( 0 ) );
}

template<class Ch> TCU bool VecCrtpMethods<Ch>::operator==( const VecCrtpMethods<U> &that ) const {
    if ( cast().size() != that.cast().size() )
        return false;
    for( PI i = 0; i < cast().size(); ++i )
        if ( ! ( operator[]( i ) == that[ i ] ) )
            return false;
    return true;
}

template<class Ch> TCU bool VecCrtpMethods<Ch>::operator<( const VecCrtpMethods<U> &that ) const {
    if ( cast().size() != that.cast().size() )
        return cast().size() < that.cast().size();
    for( PI i = 0; i < cast().size(); ++i )
        if ( operator[]( i ) != that[ i ] )
            return operator[]( i ) < that[ i ];
    return false;
}

template<class Ch> TCU auto *VecCrtpMethods<Ch>::push_back_unique( U &&val ) {
    for( unsigned i = 0; i < cast().size(); ++i )
        if ( operator[]( i ) == val )
            return &operator[]( i );
    return cast().push_back( std::forward<U>( val ) );
}

template<class Ch> TCA PI VecCrtpMethods<Ch>::push_back_ind( A &&...args ) {
    PI res = cast().size();
    cast().push_back( std::forward<A>( args )... );
    return res;
}

template<class Ch> TCU auto &VecCrtpMethods<Ch>::append( const U &vec ) {
    cast().reserve( cast().size() + vec.size() );
    for( const auto &val : vec )
        cast().push_back( val );
    return *this;
}

template<class Ch> TCU auto &VecCrtpMethods<Ch>::append( U &&vec ) {
    cast().reserve( cast().size() + vec.size() );
    for( auto &val : vec )
        cast().push_back( std::move( val ) );
    return *this;
}

template<class Ch> TCU bool VecCrtpMethods<Ch>::contains( const U &val ) const {
    for( unsigned i = 0; i < cast().size(); ++i )
        if ( operator[]( i ) == val )
            return true;
    return false;
}

template<class Ch> TCU PI VecCrtpMethods<Ch>::count( const U &val ) const {
    PI res = 0;
    for( unsigned i = 0; i < cast().size(); ++i )
        res += operator[]( i ) == val;
    return res;
}

template<class Ch> auto VecCrtpMethods<Ch>::pop_back_val() {
    auto res = std::move( cast().back() );
    cast().pop_back();
    return res;
}

template<class Ch> void VecCrtpMethods<Ch>::remove( unsigned pos ) {
    for( PI j = pos; j + 1 < cast().size(); ++j )
        operator[]( j ) = std::move( operator[]( j + 1 ) );
    cast().pop_back();
}

template<class Ch> TCU PI VecCrtpMethods<Ch>::index_of( const U &val ) const {
    for( unsigned i = 0; i < cast().size(); ++i )
        if ( operator[]( i ) == val )
            return i;
    return cast().size();
}

template<class Ch> TCU auto VecCrtpMethods<Ch>::filter( U &&f ) const {
    using R = typename std::decay<decltype( f( *cast().begin() ) )>::type;
    auto res = Vec<R,Ch::static_size>::from_reservation( cast().size() );
    for( const auto &val : cast() )
        if ( f( val ) )
            res.push_back_in_reserved( val );
    return res;
}

template<class Ch> TCU auto VecCrtpMethods<Ch>::map( U &&f ) const {
    using R = typename std::decay<decltype( f( *cast().begin() ) )>::type;
    auto res = Vec<R,Ch::static_size>::from_reservation( cast().size() );
    for( auto &&val : cast() )
        res.push_back_in_reserved( f( val ) );
    return res;
}


#pragma once

#include "../types/TypeConfig.h"
#include <functional>

template<class T,int size=-1,int rese=-1>
class Vec;

/**
 *
*/
template<class Ch>
class VecCrtpMethods {
public:
    struct          Function     {};

    const Ch&       cast                     () const;
    Ch&             cast                     ();

    const auto&     operator[]               ( PI index ) const;
    auto&           operator[]               ( PI index );

    TCAB auto       reduction                ( const A &self_func, B init ) const;
    auto            product                  () const;
    auto            sum                      () const;

    TCU bool        contains                 ( const U &val ) const;
    TCU PI          count                    ( const U &val ) const;

    TCU bool        operator==               ( const VecCrtpMethods<U> &that ) const;
    TCU bool        operator<                ( const VecCrtpMethods<U> &that ) const;

    TCU auto*       push_back_unique         ( U &&val );
    TCA PI          push_back_ind            ( A &&...args ); ///< push back and get index of item (i.e. size before the push)
    TCU auto&       append                   ( const U &vec );
    TCU auto&       append                   ( U &&vec );

    auto            pop_back_val             ();
    void            remove                   ( unsigned pos );

    TCU PI          index_of                 ( const U &val ) const;

    TCU auto        filter                   ( U &&f ) const;
    TCU auto        map                      ( U &&f ) const;

    TCU const auto* first_checking           ( U &&fnc ) const { for( unsigned i = 0; i < cast().size(); ++i ) if ( fnc( operator[]( i ) ) ) return &operator[]( i ); return (const typename Ch::T *)nullptr; }
    TCU auto*       first_checking           ( U &&fnc ) { for( unsigned i = 0; i < cast().size(); ++i ) if ( fnc( operator[]( i ) ) ) return &operator[]( i ); return (typename Ch::T *)nullptr; }

    TCA auto&       secure_get               ( PI index, const A &...def ) { if ( index >= cast().size() ) cast().resize( index + 1, def... ); return operator[]( index ); }

    TCU void        remove_if                ( U &&f ) { for( unsigned i = 0; i < cast().size(); ++i ) if ( f( operator[]( i ) ) ) remove( i-- );  }

    //    void            erase                    ( ST pos ) { for( ST j = pos; j < _size - 1; ++j ) _data[ j ] = std::move( _data[ j + 1 ] ); pop_back(); }

    //    TCU void        remove_first_checking    ( U &&op ) { for( unsigned i = 0; i < size(); ++i ) if ( op( operator[]( i ) ) ) return remove( i ); }
    //    void            remove_duplicates        () { for( unsigned i = 0; i < size(); ++i ) for( unsigned j = i + 1; j < size(); ++j ) if ( operator[]( i ) == operator[]( j ) ) remove( j-- ); }
    //    void            remove_unordered         ( unsigned pos ) { if ( pos + 1 != size() ) operator[]( pos ) = std::move( back() ); pop_back(); }
    //    void            remove_first             ( const T &val ) { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return remove( i ); }

    //    TCU ST          index_first_checking     ( U &&op ) const { for( unsigned i = 0; i < size(); ++i ) if ( op( operator[]( i ) ) ) return i; return size(); }
    //    TCU const T*    find                     ( const U &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return &operator[]( i ); return nullptr; }
    //    TCU T*          find                     ( const U &val ) { for( unsigned i = 0; i < size(); ++i ) if ( operator[]( i ) == val ) return &operator[]( i ); return nullptr; }


    //    Vec             without                  ( unsigned index             ) const { Vec res; res.reserve( size() - 1   ); for( unsigned i = 0  ; i < size(); ++i ) if ( i != index ) res << operator[]( i ); return res; }
    //    Vec             subvec                   ( unsigned beg, unsigned end ) const { Vec res; res.reserve( end - beg    ); for( unsigned i = beg; i < end   ; ++i ) res << operator[]( i ); return res; }
    //    Vec             up_to                    ( unsigned end               ) const { Vec res; res.reserve( end          ); for( unsigned i = 0  ; i < end   ; ++i ) res << operator[]( i ); return res; }
    //    Vec             from                     ( unsigned beg               ) const { Vec res; res.reserve( size() - beg ); for( unsigned i = beg; i < size(); ++i ) res << operator[]( i ); return res; }

    //    bool            all_eq                   () const { for( unsigned i = 1; i < size(); ++i ) if ( operator[]( i ) != operator[]( 0 ) ) return false; return true; }

    //    void            secure_set               ( unsigned index, const T &val ) { if ( index >= size() ) resize( index + 1 ); operator[]( index ) = val; }

    //    TCU bool        contains_checking        ( const U &fnc ) const { for( unsigned i = 0; i < size(); ++i ) if ( fnc( operator[]( i ) ) ) return true; return false; }
    //    bool            only_has                 ( const T &val ) const { for( unsigned i = 0; i < size(); ++i ) if ( _data[ i ] != val ) return false; return true; }

    //    Vec             uniqued                  () const { Vec res; for( const T &val : *this ) res.push_back_unique( val ); return res; }
    //    Vec             concat                   ( const Vec &v ) const { Vec res = *this; return res.append( v ); }

};

//template<class T,int size,class Device>
//static Vec<T,size,Device> deserialize( S<Vec<T,size,Device>>, std::istream &is ) {
//    Vec<T,size,Device> res;
//    if ( size < 0 )
//        res.resize( deserialize<ST>( is ) );
//    for( ST i = 0; i < res.size(); ++i )
//        res[ i ] = deserialize<T>( is );
//    return res;
//}

//template<class T,int size,class Device>
//void display  ( std::ostream &os, const Vec<T,size,Device> &vec ) {
//    if ( size < 0 )
//        serialize( os, vec.size() );
//    for( const T &val : vec )
//        serialize( os, val );
//}

#include "VecCrtpMethods.tcc"

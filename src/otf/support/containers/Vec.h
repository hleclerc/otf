#pragma once

#include "VecCrtpMethods.h"
#include "DataPtr.h"
#include <utility>

/**
 * Dynamic version
*/
template<class _T>
class Vec<_T,-1,-1> : public VecCrtpMethods<Vec<_T,-1,-1>> {
public:
    using           Ancestor             = VecCrtpMethods<Vec<_T,-1,-1>>;
    using           Function             = typename Ancestor::Function;

    enum {          static_size          = -1 };
    using           Data                 = DataPtr<_T,-1,1>;
    using           T                    = _T;

    static Vec      from_reservation     ( PI rese );
    TCU static Vec  from_iterable        ( const U &iterable, PI size );
    TCU static Vec  from_iterable        ( const U &iterable );
    TCU static Vec  from_function        ( PI size, U &&func );
    TCA static Vec  from_values          ( A &&...values );
    static Vec      from_size            ( PI size );

    TCU             Vec                  ( std::initializer_list<U> &&that );
    TCU             Vec                  ( Function, PI size, U &&func );
    TCU             Vec                  ( Fill, PI size, U &&value );
    /**/            Vec                  ( const Vec &that );
    /**/            Vec                  ( Vec &&that );
    TCU             Vec                  ( U &&that );
    /**/            Vec                  ();
    /**/           ~Vec                  ();

    Vec&            operator=            ( const Vec &that );
    Vec&            operator=            ( Vec &&that );
    TCU Vec&        operator=            ( U &&that );

    const T&        at                   ( PI index ) const;
    T&              at                   ( PI index );

    T*              begin                () const { return _data.ptr; }
    T*              end                  () const { return _data.ptr + _size; }

    bool            empty                () const;
    PI              size                 () const;

    PI              reserved_bytes       () const;
    PI              reserved             () const;

    T&              back                 () const { return _data.ptr[ _size - 1 ]; }

    Data            moved_data           ();
    Data            data                 () const { return _data; }
    T*              ptr                  () const { return _data.ptr; }

    Vec&            operator<<           ( const T &val );
    Vec&            operator<<           ( T &&val );

    TCA T*          push_back_in_reserved( A &&...args );
    TCA T*          push_back            ( A &&...args );
    void            reserve              ( PI rese );
    TCA void        resize               ( PI size, A &&...args );
    TCU T*          insert               ( PI pos, U &&val );
    void            clear                () { for( PI i = _size; i--; ) _data.ptr[ i ].~T(); _size = 0; }

    void            pop_back             ();


protected:
    void            _init_rese           ( PI nb_items );
    void            _init_size           ( PI nb_items );
    static Data     _allocate            ( PI nb_items );
    static void     _delete              ( Data data, PI size );
    static void     _free                ( Data data );

    TCU auto        _init_from           ( U &&that ) -> decltype( std::begin( that ), std::end( that ), std::size( that ), void() );

    Data            _data;
    PI              _size;
};

/**
 * Static version
*/
template<class _T,int _size>
class Vec<_T,_size,-1> : public VecCrtpMethods<Vec<_T,_size,-1>> {
public:
    using           Ancestor     = VecCrtpMethods<Vec<_T,_size,-1>>;
    using           Function     = typename Ancestor::Function;

    enum {          static_size  = _size };
    using           T            = _T;

    TCU static Vec  from_function( U &&func );
    static Vec      zeroes       ();

    TCU             Vec          ( std::initializer_list<U> &&that );
    TCU             Vec          ( Function, U &&func );
    TCA             Vec          ( Fill, A&&...args );
    /**/            Vec          ( const Vec &that );
    TCU             Vec          ( const U &that );
    /**/            Vec          ( Vec &&that );
    TCU             Vec          ( U &&that );
    /**/            Vec          ();
    /**/           ~Vec          ();

    Vec&            operator=    ( const Vec &that );
    TCU Vec&        operator=    ( const U &that );
    Vec&            operator=    ( Vec &&that );
    TCU Vec&        operator=    ( U &&that );

    const T&        at           ( PI index ) const;
    T&              at           ( PI index );

    bool            empty        () const;
    PI              size         () const;

    const T*        ptr          () const { return reinterpret_cast<const T *>( _data ); }
    T*              ptr          () { return reinterpret_cast<T *>( _data ); }

    const T*        begin        () const { return ptr(); }
    T*              begin        () { return ptr(); }

    const T*        end          () const { return ptr() + _size; }
    T*              end          () { return ptr() + _size; }

protected:
    TCU auto        _init_from   ( const U &that ) -> decltype( std::begin( that ), std::end( that ), std::size( that ), void() );
    TCU auto        _init_from   ( U &&that ) -> decltype( std::begin( that ), std::end( that ), std::size( that ), void() );

    char            _data        [ sizeof( T ) * _size ];
};

#include "Vec.tcc" // IWYU pragma: export

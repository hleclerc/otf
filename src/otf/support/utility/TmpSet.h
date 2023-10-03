#pragma once

#include <utility>

/**
*/
template<class T>
struct TmpSet {
    /**/  TmpSet( T &ref, T new_value ) : old_value( ref ), ref( ref ) { ref = new_value; }
    /**/  TmpSet( T &ref ) : old_value( ref ), ref( ref ) {}

    /**/ ~TmpSet() { ref = std::move( old_value ); }

    T     old_value;
    T&    ref;
};

template<class T,class A,class F>
auto tmp_set( T &ref, A &&val, F &&func ) {
    TmpSet<T> raii_save{ ref };
    ref = std::forward<A>( val );
    return func();
}

template<class T,class F>
auto tmp_set( T &ref, F &&func ) {
    TmpSet<T> raii_save{ ref };
    return func();
}


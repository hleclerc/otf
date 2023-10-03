#pragma once

#include "../display/Displayer.h"
#include <utility>

/**
 * TODO: uninitialized content
*/
template<class T>
struct Opt {
    /**/     Opt          ( const Opt<T> &that ) : content( that.content ), ok( that.ok ) {}
    /**/     Opt          ( Opt<T> &&that ) : content( std::move( that.content ) ), ok( std::exchange( that.ok, false ) ) {}
    /**/     Opt          ( const T &content ) : content( content ), ok( true ) {}
    /**/     Opt          ( T &&content ) : content( std::move( content ) ), ok( true ) {}
    /**/     Opt          () : ok( false ) {}

    void     display      ( Displayer &sr ) const { if ( ok ) return sr << content; sr << "none"; }

    Opt&     operator=    ( const Opt<T> &that ) { content = that.content; ok = that.ok; return *this; }
    Opt&     operator=    ( Opt<T> &&that ) { content = std::move( that.content ); ok = std::exchange( that.ok, false ); return *this; }
    Opt&     operator=    ( const T &that ) { content = that; ok = true; return *this; }
    Opt&     operator=    ( T &&that ) { content = std::move( that ); ok = true; return *this; }

    explicit operator bool() const { return ok; }
    const T *operator->   () const { return &content; }
    const T &operator*    () const { return content; }

    void     clear        () { ok = false; }

    T        content;     ///<
    bool     ok;          ///<
};

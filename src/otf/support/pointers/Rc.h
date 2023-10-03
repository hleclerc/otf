#pragma once

// #include "../display/Displayer.h"
#include "../types/TypeConfig.h"

/**
 *
 */
template<class T>
struct Rc {
    template<class U>     Rc         ( const Rc<U> &obj );
    /**/                  Rc         ( const Rc &obj );
    template<class U>     Rc         ( Rc<U> &&obj );
    /**/                  Rc         ( Rc &&obj );
    template<class U>     Rc         ( U *obj );
    /**/                  Rc         ();

    /**/                 ~Rc         ();

    TCU static Rc         from_ptr   ( U *ptr );

    T*                    operator-> () const;
    T&                    operator*  () const;
    T*                    get        () const;

    //void                display    ( Displayer &sr ) const { sr.add_ptr( data ); }
    explicit operator     bool       () const;

    template<class U> Rc& operator=  ( const Rc<U> &obj );
    Rc&                   operator=  ( const Rc &obj );
    template<class U> Rc& operator=  ( Rc<U> &&obj );
    Rc&                   operator=  ( Rc &&obj );

    bool                  operator== ( const Rc<T> &p ) const;
    bool                  operator!= ( const Rc<T> &p ) const;
    bool                  operator<  ( const Rc<T> &p ) const;
    bool                  operator<= ( const Rc<T> &p ) const;
    bool                  operator>  ( const Rc<T> &p ) const;
    bool                  operator>= ( const Rc<T> &p ) const;

    void                  clear      ();

    static void           inc_ref    ( T *data );
    static void           dec_ref    ( T *data );

    void                  inc_ref    ();
    void                  dec_ref    ();

    T*                    data;      ///<
};

#include "Rc.tcc" // IWYU pragma: export

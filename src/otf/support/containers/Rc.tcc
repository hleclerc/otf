#pragma once

#include <utility>
#include "Rc.h"

template<class T>
Rc<T>::Rc() : data( nullptr ) {
}

template<class T>
Rc<T>::Rc( Rc &&obj ) : data( std::exchange( obj.data, nullptr ) ) {
}

template<class T>
Rc<T>::Rc( const Rc &obj ) : data( obj.data ) {
    inc_ref( data );
}

template<class T> template<class U>
Rc<T>::Rc( const Rc<U> &obj ) : data( obj.data ) {
    inc_ref( data );
}

template<class T> template<class U>
Rc<T>::Rc( Rc<U> &&obj ) : data( std::exchange( obj.data, nullptr ) ) {
}

template<class T> template<class U>
Rc<T>::Rc( U *obj ) : data( obj ) {
    inc_ref( data );
}

template<class T>
Rc<T>::~Rc() {
    dec_ref( data );
}

template<class T> TCU
Rc<T> Rc<T>::from_ptr( U *ptr ) {
    inc_ref( ptr );

    Rc res;
    res.data = ptr;
    return res;
}

template<class T>
Rc<T> &Rc<T>::operator=( const Rc &obj ) {
    inc_ref( obj.data );
    dec_ref( data );
    data = obj.data;
    return *this;
}

template<class T> template<class U>
Rc<T> &Rc<T>::operator=( const Rc<U> &obj ) {
    inc_ref( obj.data );
    dec_ref( data );
    data = obj.data;
    return *this;
}

template<class T>
Rc<T> &Rc<T>::operator=( Rc &&obj ) {
    dec_ref( data );
    data = std::exchange( obj.data, nullptr );
    return *this;
}

template<class T> template<class U>
Rc<T> &Rc<T>::operator=( Rc<U> &&obj ) {
    dec_ref( data );
    data = std::exchange( obj.data, nullptr );
    return *this;
}


template<class T>
Rc<T>::operator bool() const {
    return data;
}

template<class T>
void Rc<T>::clear() {
    dec_ref( std::exchange( data, nullptr ) );
}

template<class T>
bool Rc<T>::operator==( const Rc<T> &p ) const { return data == p.data; }

template<class T>
bool Rc<T>::operator!=( const Rc<T> &p ) const { return data != p.data; }

template<class T>
bool Rc<T>::operator< ( const Rc<T> &p ) const { return data <  p.data; }

template<class T>
bool Rc<T>::operator<=( const Rc<T> &p ) const { return data <= p.data; }

template<class T>
bool Rc<T>::operator> ( const Rc<T> &p ) const { return data >  p.data; }

template<class T>
bool Rc<T>::operator>=( const Rc<T> &p ) const { return data >= p.data; }

template<class T>
T *Rc<T>::get() const { return data; }

template<class T>
T *Rc<T>::operator->() const { return data; }

template<class T>
T &Rc<T>::operator*() const { return *data; }

template<class T>
void Rc<T>::inc_ref( T *data ) {
    if ( data )
        ++data->ref_count;
}

template<class T>
void Rc<T>::dec_ref( T *data ) {
    if ( data && --data->ref_count == 0 )
        delete data;
}

template<class T>
void Rc<T>::inc_ref() {
    inc_ref( data );
}

template<class T>
void Rc<T>::dec_ref() {
    dec_ref( data );
}

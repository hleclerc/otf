#pragma once

#include "Vec.h" // IWYU pragma: keep

/**
*/
template<class T,int s = 256>
class StableVec {
public:
    struct      CstIter       { const StableVec &v; PI ind; void operator++() { ++ind; } const T &operator*() const { return v[ ind ]; } bool operator!=( const CstIter &that ) const { return ind != that.ind; } };

    /**/        StableVec();
    /**/       ~StableVec();

    TCA T&      secure_get( PI index, const A &...default_value );
    const T&    operator[]( PI index ) const;
    T&          operator[]( PI index );
    PI          size      () const;

    CstIter     begin     () const { return { *this, 0 }; }
    CstIter     end       () const { return { *this, _size }; }

    TCA T*      push_back ( A &&...args );

private:
    TCA void    _add_bucket( PI size, const A &...default_value );

    Vec<char *> _buckets;
    PI          _size;
};

#include "StableVec.tcc" // IWYU pragma: export

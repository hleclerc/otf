#pragma once

#include "../types/TypeConfig.h"

/**
*/
template<class A>
class IntrusiveList {
public:
    /**/     IntrusiveList();

    TCT T*   add( T *v );

    A*       last;
};

#include "IntrusiveList.tcc" // IWYU pragma: export

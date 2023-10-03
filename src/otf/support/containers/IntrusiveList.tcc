#pragma once

#include "IntrusiveList.h"
#include <utility>

template<class A>
IntrusiveList<A>::IntrusiveList() {
    last = nullptr;
}

template<class A> TCT
T *IntrusiveList<A>::add( T *res ) {
    res->prev_in_list = last;
    last = res;

    return res;
}

#pragma once

#include <algorithm>

template<class T,class U>
T &self_max( T &dst, const U &val ) {
    using std::max;
    dst = max( dst, val );
    return dst;
}

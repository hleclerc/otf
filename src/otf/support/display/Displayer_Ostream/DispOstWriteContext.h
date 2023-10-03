#pragma once

#include "../../types/TypeConfig.h"
#include <ostream>
#include <set>
class Displayer_Ostream;

/**
*/
struct DispOstWriteContext {
    void               inc_level();
    void               dec_level();
    void               write_sep();

    Displayer_Ostream* ds;
    std::ostream*      os;

    std::set<String>   seen_ptrs;

    bool               hide_default_values = false;
    int                compacity = 0;
    int                nsp = 0;
};

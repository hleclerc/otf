#pragma once

#include "../../containers/Vec.h"
#include <ostream>
class Displayer_Ostream;

/**
*/
struct DispOstWriteDotContext {
    String             attr_name  () const;

    TCT std::ostream&  operator<< ( const T &value ) const { return *label << value; }
    String             dot_corr   ( const String &str ) const;
    void               add_td     ( StringView str ) const;

    Vec<String>        attr_names;
    String             ptr_repr;
    std::ostream*      label;
    std::ostream*      glob;
    Displayer_Ostream* ds;
};


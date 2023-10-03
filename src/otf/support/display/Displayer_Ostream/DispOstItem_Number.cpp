#include "DispOstItem_Number.h"

DispOstItem_Number::DispOstItem_Number( String &&repr, StringView type ) : repr( repr ), type( type ) {
}

bool DispOstItem_Number::has_default_value() const {
    if ( type == "Bool" && repr == "0" )
        return true;
    return DispOstItem::has_default_value();
}

void DispOstItem_Number::write_dot_label( DispOstWriteDotContext &wc ) {
    wc.add_td( repr );
}

void DispOstItem_Number::write( DispOstWriteContext &wc ) {
    *wc.os << repr;
}

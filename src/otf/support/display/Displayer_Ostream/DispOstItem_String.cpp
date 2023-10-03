#include "DispOstItem_String.h"

DispOstItem_String::DispOstItem_String( StringView repr ) : repr( repr ) {
}

bool DispOstItem_String::has_default_value() const {
    return repr.empty();
}

void DispOstItem_String::write_dot_label( DispOstWriteDotContext &wc ) {
    wc.add_td( repr );
}

void DispOstItem_String::write( DispOstWriteContext &wc ) {
    *wc.os << repr;
}

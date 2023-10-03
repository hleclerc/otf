#include "DispOstItem_Array.h"

DispOstItem_Array::DispOstItem_Array() {
}

bool DispOstItem_Array::has_default_value() const {
    return items.empty();
}

void DispOstItem_Array::write_dot_label( DispOstWriteDotContext &wc ) {
    wc << "<TD BGCOLOR='red' WIDTH='2' CELLPADDING='0' PORT='" << wc.attr_name() << "'></TD>";
    for( PI i = 0; i < items.size(); ++i ) {
        wc.attr_names << std::to_string( i );
        items[ i ]->write_dot_label( wc );
        wc.attr_names.pop_back();
    }
    wc << "<TD BGCOLOR='blue' WIDTH='2' CELLPADDING='0'></TD>";
}

void DispOstItem_Array::write( DispOstWriteContext &wc ) {
    if ( items.empty() ) {
        *wc.os << "[]";
    } else {
        if ( wc.compacity == 0 ) {
            wc.inc_level();
            for( PI i = 0; i < items.size(); ++i ) {
                wc.write_sep();
                items[ i ]->write( wc );
            }
            wc.dec_level();
        } else {
            *wc.os << "[ ";
            for( PI i = 0; i < items.size(); ++i ) {
                if ( i )
                    *wc.os << ", ";
                items[ i ]->write( wc );
            }
            *wc.os << " ]";
        }
    }
}

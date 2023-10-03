#include "DispOstItem_Object.h"

DispOstItem_Object::DispOstItem_Object( StringView type, bool is_polymorphic ) : is_polymorphic( is_polymorphic ), type( type ) {
}

bool DispOstItem_Object::has_default_value() const {
    if ( is_polymorphic )
        return false;
    for( const auto &p : items )
        if ( ! p->has_default_value() )
            return false;
    return true;
}

void DispOstItem_Object::write_dot_label( DispOstWriteDotContext &wc ) {
    if ( is_polymorphic )
        wc.add_td( type );
    for( const auto &p : items )
        p->write_dot_label( wc );
}

void DispOstItem_Object::write( DispOstWriteContext &wc ) {
    *wc.os << type;

    if ( items.empty() ) {
        *wc.os << "{}";
    } else {
        if ( wc.compacity == 0 ) {
            wc.inc_level();
            for( PI i = 0; i < items.size(); ++i ) {
                if ( wc.hide_default_values && items[ i ]->has_default_value() )
                    continue;
                wc.write_sep();
                items[ i ]->write( wc );
            }
            wc.dec_level();
        } else {
            *wc.os << "{ ";
            for( PI i = 0, c = 0; i < items.size(); ++i ) {
                if ( wc.hide_default_values && items[ i ]->has_default_value() )
                    continue;
                if ( c++ )
                    *wc.os << ", ";
                items[ i ]->write( wc );
            }
            *wc.os << " }";
        }

    }
}

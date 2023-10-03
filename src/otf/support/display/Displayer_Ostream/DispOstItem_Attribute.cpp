#include "DispOstItem_Attribute.h"
#include "../../utility/TmpSet.h"

DispOstItem_Attribute::DispOstItem_Attribute( StringView name ) : name( name ) {
}

bool DispOstItem_Attribute::has_default_value() const {
    ASSERT( items.size() == 1 );
    return items[ 0 ]->has_default_value();
}

void DispOstItem_Attribute::write_dot_label( DispOstWriteDotContext &wc ) {
    tmp_set( wc.attr_names, [&]() {
        wc.attr_names << name;
        ASSERT( items.size() == 1 );
        items[ 0 ]->write_dot_label( wc );
    } );
}

void DispOstItem_Attribute::write( DispOstWriteContext &wc ) {
    *wc.os << name << ": ";
    for( auto &item : items )
        item->write( wc );
}

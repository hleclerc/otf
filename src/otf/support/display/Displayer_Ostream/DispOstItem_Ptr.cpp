#include "DispOstItem_Ptr.h"
#include "Displayer_Ostream.h"

DispOstItem_Ptr::DispOstItem_Ptr( String &&ptr_repr, PtrInfo &&ptr_info, Items *items ) : ptr_repr( std::move( ptr_repr ) ), ptr_info( std::move( ptr_info ) ), items( items ) {
}

bool DispOstItem_Ptr::has_default_value() const {
    return items == nullptr;
}

void DispOstItem_Ptr::write_dot_label( DispOstWriteDotContext &wc ) {
    wc.add_td( "" );

    if ( items ) {
        std::ostream &es = *wc.glob;

        es << "    " << wc.ptr_repr;
        if ( wc.attr_names.size() )
            es << ":\"" << wc.attr_name() << "\"";

        es << " -> " << ptr_repr;
        if ( ptr_info.port.size() )
            es << ":\"" << ptr_info.port << "\"";

        int cpt = 0;
        if ( ptr_info.edge_style.size() )
            es << ( cpt++ ? "," : "[" ) << "style=\"" << ptr_info.edge_style << "\"";
        if ( ptr_info.edge_label.size() )
            es << ( cpt++ ? "," : "[" ) << "label=\"" << wc.dot_corr( ptr_info.edge_label ) << "\"";
        if ( cpt )
            es << "]";
        es << "\n";
    }
}

void DispOstItem_Ptr::write( DispOstWriteContext &wc ) {
    if ( items ) {
        const auto &ptr_data = wc.ds->ptr_map[ ptr_repr ];
        if ( ptr_data.count > 1 ) {
            // already seen ?
            if ( ! wc.seen_ptrs.insert( ptr_repr ).second ) {
                *wc.os << "*" << ptr_repr;
                if ( ptr_info.port.size() )
                    *wc.os << ":" << ptr_info.port;
                return;
            }

            // write pos
            *wc.os << "&(" << ptr_repr << ": ";
        }

        for( PI i = 0; i < items->size(); ++i ) {
            if ( i )
                *wc.os << ", ";
            items->at( i )->write( wc );
        }

        if ( ptr_data.count > 1 ) {
            *wc.os << ")";
            if ( ptr_info.port.size() )
                *wc.os << ": " << ptr_info.port;
        }
    } else {
        *wc.os << "NULL";
    }

}

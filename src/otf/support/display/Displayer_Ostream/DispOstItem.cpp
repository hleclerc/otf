#include "DispOstItem.h"
#include "../../utility/TmpSet.h"
#include <sstream>

DispOstItem::~DispOstItem() {
}

bool DispOstItem::has_default_value() const {
    return false;
}

void DispOstItem::write_dot( DispOstWriteDotContext &wc, const String &ptr_repr ) {
    // write labels in a local stringstream
    std::ostringstream ss;
    tmp_set( wc.label, &ss, [&]() {
        wc << "    " << ptr_repr << " [label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING='4' PORT='_root'><TR>";
        tmp_set( wc.ptr_repr, [&]() {
            wc.ptr_repr = ptr_repr;
            tmp_set( wc.attr_names, [&]() {
                wc.attr_names.clear();
                write_dot_label( wc );
            } );
        } );
        wc << "</TR></TABLE>>,shape=\"plaintext\"];\n";
    } );

    *wc.glob << ss.str();
}

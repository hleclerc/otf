#include "DispOstItem_Attribute.h"
#include "DispOstItem_Object.h"
#include "DispOstItem_String.h"
#include "DispOstItem_Number.h"
#include "DispOstItem_Array.h"
#include "DispOstItem_Ptr.h"
#include "Displayer_Ostream.h"
#include <fstream>

Displayer_Ostream::Displayer_Ostream() {
    stack << &root_list;
}

void Displayer_Ostream::beg_attribute( StringView name ) {
    auto ptr = std::make_unique<DispOstItem_Attribute>( name );
    stack << &ptr->items;

    *stack[ stack.size() - 2 ] << std::move( ptr );
}

void Displayer_Ostream::end_attribute() {
    stack.pop_back();
}

void Displayer_Ostream::beg_object( StringView type, bool is_polymorphic ) {
    auto ptr = std::make_unique<DispOstItem_Object>( type, is_polymorphic );
    stack << &ptr->items;

    *stack[ stack.size() - 2 ] << std::move( ptr );
}

void Displayer_Ostream::end_object() {
    stack.pop_back();
}

void Displayer_Ostream::beg_array() {
    auto ptr = std::make_unique<DispOstItem_Array>();
    stack << &ptr->items;

    *stack[ stack.size() - 2 ] << std::move( ptr );
}

void Displayer_Ostream::end_array() {
    stack.pop_back();
}

void Displayer_Ostream::add_string( StringView value ) {
    *stack.back() << std::make_unique<DispOstItem_String>( value );
}

void Displayer_Ostream::add_ptr( String &&ptr_repr, PtrInfo &&ptr_info, bool is_valid, const std::function<void()> &cb ) {
    // get data from ptr if not already done
    Vec<ItemPtr> *items = nullptr;
    if ( is_valid ) {
        bool known = ptr_map.contains( ptr_repr );
        PtrData &pd = ptr_map[ ptr_repr ];

        if ( ! known ) {
            pd.count = 0;

            stack << &pd.items;
            cb();
            stack.pop_back();
        }

        items = &pd.items;
        ++pd.count;
    }

    // reg ptr
    *stack.back() << std::make_unique<DispOstItem_Ptr>( std::move( ptr_repr ), std::move( ptr_info ), items );
}

void Displayer_Ostream::add_number( String &&repr, StringView type ) {
    *stack.back() << std::make_unique<DispOstItem_Number>( std::move( repr ), type );
}

void Displayer_Ostream::write( std::ostream &os ) {
    DispOstWriteContext wc{ this, &os };
    int cpt = 0;
    for( const ItemPtr &item : root_list ) {
        if ( cpt++ )
            *wc.os << ' ';
        item->write( wc );
    }
}

int Displayer_Ostream::exec_dot( String f ) {
    // call graphviz
    String cmd = "dot -Tpdf " + f + " > " + f + ".pdf";
    if ( int res = system( cmd.c_str() ) )
        return res;

    // call viewer
    cmd = "l=`qdbus --session | grep okular`;\nu=`for i in $l; do gdbus call --session --dest $i --object-path /okular --method org.kde.okular.currentDocument; done`;\n( echo $u | grep " + f + ".pdf ) || okular " + f + ".pdf &";
    if ( int res = system( cmd.c_str() ) )
        std::cerr << "error calling pdf display (" << res << ")" << std::endl;
    return 0;
}

void Displayer_Ostream::show() {
    String filename = ".graph.dot";
    std::ofstream os( filename );
    DispOstWriteDotContext wc{ .label = nullptr, .glob = &os, .ds = this };

    os << "digraph SmurfGraphol {\n";

    ASSERT( root_list.size() == 1 );
    root_list[ 0 ]->write_dot( wc, "root" );

    for( auto &p : ptr_map ) {
        ASSERT( p.second.items.size() == 1 );
        p.second.items[ 0 ]->write_dot( wc, p.first );
    }

    os << "}\n";

    os.close();

    exec_dot( filename );

}

#define BASE_TYPE( NAME, ... ) void Displayer_Ostream::add_number( NAME value ) { add_number( std::to_string( value ), #NAME ); }
#include "../../types/base_arithmetic_types.h"
#undef BASE_TYPE

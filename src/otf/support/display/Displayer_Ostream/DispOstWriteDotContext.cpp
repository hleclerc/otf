#include "DispOstWriteDotContext.h"
#include "../../string/join.h"

String DispOstWriteDotContext::attr_name() const {
    auto is_a_number = []( char c ) { return c >= '0' && c <= '9'; };
    String tmp = join( attr_names, "." );

    if ( tmp.size() && is_a_number( tmp[ 0 ] ) )
        tmp = "_" + tmp;

    String res;
    res.reserve( tmp.size() );
    for( char c : tmp ) {
        switch ( c ) {
        case '&': res += '_'; break;
        case '-': res += '_'; break;
        case '>': res += '_'; break;
        default: res += c;
        }
    }

    return res;
}

String DispOstWriteDotContext::dot_corr( const String &str ) const {
    String res;
    for( char c : str ) {
        switch ( c ) {
        case ':': res += '_'; break;
        default:  res += c;
        }
    }
    return res;
}

void DispOstWriteDotContext::add_td( StringView str ) const {
    *label << "<TD port='" << attr_name() << "'>" << str << "</TD>";
}

#pragma once

#include "ptr_repr.h"
#include <functional>
#include <iostream>
#include <mutex>

/**
*/
class Displayer {
public:
    struct            PtrInfo      { String edge_style; String edge_color; String edge_label; String port; };

    virtual          ~Displayer   ();

    // single items
    TCT void          add_attribute( StringView name, T &&value ) { beg_attribute( name ); display( *this, std::forward<T>( value ) ); end_attribute(); }
    TCA void          add_object   ( StringView type, bool is_polymorphic, StringView arg_names, A &&...arg_values ) { beg_object( type, is_polymorphic ); ( add_attribute( read_arg_name( arg_names ), std::forward<A>( arg_values ) ), ... ); end_object(); }
    TCT void          add_value    ( T &&value ) { display( *this, std::forward<T>( value ) ); }
    void              add_ptr      ( std::nullptr_t, PtrInfo &&pi = {} ) { add_ptr( "nullptr", std::move( pi ), false, [&]() {} ); }
    TCT void          add_ptr      ( const T &ptr, PtrInfo &&pi = {} ) { add_ptr( ptr_repr( ptr ), std::move( pi ), ptr, [&]() { add_value( *ptr ); } ); }

    #define           BASE_TYPE( NAME, ... ) virtual void add_number( NAME value ) = 0;
    #include          "../types/base_arithmetic_types.h"
    #undef            BASE_TYPE

    virtual void      add_number   ( Byte value );

    virtual void      add_string   ( StringView value ) = 0;
    virtual void      add_ptr      ( String &&ptr_repr, PtrInfo &&pi, bool is_valid, const std::function<void()> &cb ) = 0;

    // helpers
    TCT void          operator<<   ( T &&value ) { add_value( std::forward<T>( value ) ); }

    // composite items
    virtual void      beg_attribute( StringView name ) = 0;
    virtual void      end_attribute() = 0;

    virtual void      beg_object   ( StringView type, bool is_polymorphic ) = 0;
    virtual void      end_object   () = 0;

    virtual void      beg_array    () = 0;
    virtual void      end_array    () = 0;

    // utility methods
    static StringView read_arg_name( StringView &arg_names );
};

#define SR_OBJECT( NAME, ... ) sr.add_object( #NAME, std::is_polymorphic<NAME>::value, #__VA_ARGS__, __VA_ARGS__ );

#include "display.h" // IWYU pragma: export
#include "ptr_repr.h" // IWYU pragma: export

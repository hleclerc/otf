#pragma once

#include "../../containers/Vec.h"
#include "../Displayer.h"
#include "DispOstItem.h"
#include <memory>
#include <map>

/**
*/
class Displayer_Ostream : public Displayer {
public:
    using         ItemPtr          = std::unique_ptr<DispOstItem>;
    struct        PtrData          { Vec<ItemPtr> items; PI count; };
    using         PtrMap           = std::map<String,PtrData>;
    using         Items            = Vec<ItemPtr>;

    /**/          Displayer_Ostream();

    virtual void  beg_attribute    ( StringView name ) override;
    virtual void  end_attribute    () override;

    virtual void  beg_object       ( StringView type, bool is_polymorphic = true ) override;
    virtual void  end_object       () override;

    virtual void  beg_array        () override;
    virtual void  end_array        () override;

    #define       BASE_TYPE( NAME, ... ) \
    virtual void  add_number       ( NAME value ) override;
    #include      "../../types/base_arithmetic_types.h"
    #undef        BASE_TYPE

    virtual void  add_string       ( StringView value ) override;
    virtual void  add_ptr          ( String &&ptr_repr, PtrInfo &&pi, bool is_valid, const std::function<void()> &cb ) override;

    void          add_number       ( String &&value, StringView type );

    void          write            ( std::ostream &os );
    void          show             ();

    static int    exec_dot         ( String f );

    Items         root_list;
    PtrMap        ptr_map;
    Vec<Items*>   stack;
};

#pragma once

#include "DispOstItem.h"
#include "../../containers/Vec.h"
#include "Displayer_Ostream.h"
#include <memory>

/**
*/
class DispOstItem_Ptr : public DispOstItem {
public:
    using        PtrInfo                  = Displayer_Ostream::PtrInfo;
    using        Items                    = Vec<std::unique_ptr<DispOstItem>>;

    /**/         DispOstItem_Ptr( String &&ptr_repr, PtrInfo &&ptr_info, Items *items );

    virtual bool has_default_value        () const override;
    virtual void write_dot_label          ( DispOstWriteDotContext &wc ) override;
    virtual void write                    ( DispOstWriteContext &wc ) override;

    String       ptr_repr;
    PtrInfo      ptr_info;
    Items*       items;
};

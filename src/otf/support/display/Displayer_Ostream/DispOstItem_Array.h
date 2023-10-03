#pragma once

#include "DispOstItem.h"
#include "../../containers/Vec.h"
#include <memory>

/**
*/
class DispOstItem_Array : public DispOstItem {
public:
    using        Items                      = Vec<std::unique_ptr<DispOstItem>>;

    /**/         DispOstItem_Array();

    virtual bool has_default_value          () const override;
    virtual void write_dot_label            ( DispOstWriteDotContext &wc ) override;
    virtual void write                      ( DispOstWriteContext &wc ) override;

    Items        items;
};

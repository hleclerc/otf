#pragma once

#include "../../types/TypeConfig.h"
#include "DispOstItem.h"

/**
*/
class DispOstItem_String : public DispOstItem {
public:
    /**/         DispOstItem_String( StringView repr );

    virtual bool has_default_value           () const override;
    virtual void write_dot_label             ( DispOstWriteDotContext &wc ) override;
    virtual void write                       ( DispOstWriteContext &wc ) override;

    String       repr;
};

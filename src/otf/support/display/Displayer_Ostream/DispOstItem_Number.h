#pragma once

#include "../../types/TypeConfig.h"
#include "DispOstItem.h"

/**
*/
class DispOstItem_Number : public DispOstItem {
public:
    /**/         DispOstItem_Number( String &&repr, StringView type );

    virtual bool has_default_value           () const override;
    virtual void write_dot_label             ( DispOstWriteDotContext &wc ) override;
    virtual void write                       ( DispOstWriteContext &wc ) override;

    String       repr;
    String       type;
};

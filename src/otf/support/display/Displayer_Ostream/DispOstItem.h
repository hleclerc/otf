#pragma once

#include "DispOstWriteDotContext.h"
#include "DispOstWriteContext.h"

/**
*/
class DispOstItem {
public:
    virtual     ~DispOstItem      ();

    virtual bool has_default_value() const;
    virtual void write_dot_label  ( DispOstWriteDotContext &wc ) = 0;
    virtual void write_dot        ( DispOstWriteDotContext &wc, const String &ptr_repr );
    virtual void write            ( DispOstWriteContext &wc ) = 0;
};

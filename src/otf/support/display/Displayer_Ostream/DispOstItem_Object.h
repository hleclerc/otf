#pragma once

#include "DispOstItem.h"
#include "../../containers/Vec.h"
#include <memory>

/**
*/
class DispOstItem_Object : public DispOstItem {
public:
    using        Items                       = Vec<std::unique_ptr<DispOstItem>>;

    /**/         DispOstItem_Object( StringView type, bool is_polymorphic );

    virtual bool has_default_value           () const override;
    virtual void write_dot_label             ( DispOstWriteDotContext &wc ) override;
    virtual void write                       ( DispOstWriteContext &wc ) override;

    bool         is_polymorphic;
    String       type;
    Items        items;
};

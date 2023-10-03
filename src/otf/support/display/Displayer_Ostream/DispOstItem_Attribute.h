#pragma once

#include "DispOstItem.h"
#include "../../containers/Vec.h"
#include <memory>

/**
*/
class DispOstItem_Attribute : public DispOstItem {
public:
    using        Items                          = Vec<std::unique_ptr<DispOstItem>>;

    /**/         DispOstItem_Attribute( StringView name );

    virtual bool has_default_value              () const override;
    virtual void write_dot_label                ( DispOstWriteDotContext &wc ) override;
    virtual void write                          ( DispOstWriteContext &wc ) override;

    String       name;
    Items        items;
};

#pragma once

#include "../support/types/TypeConfig.h"
class VtkOutput;

/**
*/
class PowerDiagramCell {
public:
    using        TF              = FP64;

    /**/         PowerDiagramCell();

    virtual bool is_infinite     () const = 0;
    virtual void display         ( VtkOutput &vo, TF *offset = nullptr ) = 0;
    virtual TF   volume          () = 0;

    TF           weight;         ///<
    PI           num;            ///<
};


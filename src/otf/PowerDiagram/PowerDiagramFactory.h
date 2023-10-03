#pragma once

#include "../support/pointers/Rc.h"
#include "PowerDiagram.h"

/**
*/
class PowerDiagramFactory {
public:
    using                    TF                 = PowerDiagram::TF;

    virtual                 ~PowerDiagramFactory();

    virtual Rc<PowerDiagram> create             ( PI dim, PI nb_points, const TF * const *coords, const TF *weights ) = 0;
};


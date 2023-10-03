#pragma once

#include "../support/pointers/Rc.h"
#include "PowerDiagram.h"

/**
*/
class PowerDiagramFactory {
public:
    using                    TF                 = PowerDiagram::TF;

    virtual                 ~PowerDiagramFactory();

    virtual Rc<PowerDiagram> create             ( PI dim, const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds ) = 0;
};


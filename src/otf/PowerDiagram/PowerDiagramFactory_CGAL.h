#pragma once

#include "PowerDiagramFactory.h"

/**
*/
class PowerDiagramFactory_CGAL : public PowerDiagramFactory {
public:
    virtual Rc<PowerDiagram> create( PI dim, const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds ) override;
};


#pragma once

#include "PowerDiagramFactory.h"

/**
*/
class PowerDiagramFactory_CGAL : public PowerDiagramFactory {
public:
    virtual Rc<PowerDiagram> create( PI dim, PI nb_points, const TF * const *coords, const TF *weights ) override;
};


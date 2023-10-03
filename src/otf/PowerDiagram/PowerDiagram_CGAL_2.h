#pragma once

#include "PowerDiagramCell_CGAL_2.h"
#include "PowerDiagram.h"

/**
*/
class PowerDiagram_CGAL_2 : public PowerDiagram {
public:
    using           Rt                 = PowerDiagramCell_CGAL_2::Rt;
    using           Wp                 = PowerDiagramCell_CGAL_2::Wp;
    using           Pt                 = PowerDiagramCell_CGAL_2::Pt;

    /**/            PowerDiagram_CGAL_2( const TF * const *coords, const TF *weights, PI nb_points );

    virtual void    for_each_cell      ( const std::function<void( Cell &cell, PI num_thread )> &f ) override;

    std::vector<Wp> diracs;            ///<
    Rt              rt;                ///<
};


#pragma once

#include "PowerDiagramCell_CGAL_3.h"
#include "PowerDiagram.h"

/**
*/
class PowerDiagram_CGAL_3 : public PowerDiagram {
public:
    using           Rt                 = PowerDiagramCell_CGAL_3::Rt;
    using           Wp                 = PowerDiagramCell_CGAL_3::Wp;
    using           Pt                 = PowerDiagramCell_CGAL_3::Pt;

    /**/            PowerDiagram_CGAL_3( const TF * const *coords, const TF *weights, PI nb_points );

    virtual void    for_each_cell      ( const std::function<void( Cell &cell, PI num_thread )> &f ) override;

    std::vector<Wp> diracs;            ///<
    Rt              rt;                ///<
};


#pragma once

#include "PowerDiagramCell_CGAL_2.h"
#include "PowerDiagram.h"

/**
*/
class PowerDiagram_CGAL_2 : public PowerDiagram {
public:
    using           MyVertex           = PowerDiagramCell_CGAL_2::MyVertex;
    using           MyFace             = PowerDiagramCell_CGAL_2::MyFace;
    using           Rt                 = PowerDiagramCell_CGAL_2::Rt;
    using           Wp                 = PowerDiagramCell_CGAL_2::Wp;
    using           Pt                 = PowerDiagramCell_CGAL_2::Pt;

    /**/            PowerDiagram_CGAL_2( const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds );

    virtual void    for_each_point     ( const std::function<void( PI num_point, const Vec<PI> &connected_items, PI num_thread )> &f ) override;
    virtual void    for_each_cell      ( const std::function<void( Cell &cell, PI num_thread )> &f ) override;

    std::vector<TF> boundary_coeff_x;  ///<
    std::vector<TF> boundary_coeff_y;  ///<
    std::vector<TF> boundary_offsets;  ///<
    Rt              rt;                ///<
};


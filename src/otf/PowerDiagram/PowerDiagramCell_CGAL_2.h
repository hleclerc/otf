#pragma once

//// nsmake avoid_inc CGAL/
//// nsmake lib_name gmp

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_2.h>
#include "../support/containers/Vec.h"
#include "PowerDiagramCell.h"

/**
*/
class PowerDiagramCell_CGAL_2 : public PowerDiagramCell {
public:
    using            Ck                 = CGAL::Exact_predicates_inexact_constructions_kernel;
    using            Rt                 = CGAL::Regular_triangulation_2<Ck>;
    using            Wp                 = typename Rt::Weighted_point;
    using            Pt                 = typename Rt::Point_2;

    using            Vertex             = Rt::Vertex_handle;
    using            Edge               = Rt::Edge;

    virtual bool     is_infinite        () const override;
    virtual void     display            ( VtkOutput &vo, TF *offset = nullptr ) override;
    virtual TF       volume             () override;

    void             for_each_edge_point( const std::function<void( const Pt &pt )> &f );
    void             cut                ( Vec<Pt> &pts, Pt dir, TF off ) const;

    static TF        sp                 ( const Pt &a, const Pt &b );
    static TF        n2                 ( const Pt &p );

    std::vector<TF>* boundary_coeff_x;  ///<
    std::vector<TF>* boundary_coeff_y;  ///<
    std::vector<TF>* boundary_offsets;  ///<
    Rt*              rt;                ///<
    Vertex           v;                 ///<
};


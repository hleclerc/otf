#pragma once

//// nsmake avoid_inc CGAL/
//// nsmake lib_name gmp

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_2.h>
#include "PowerDiagramCell.h"

/**
*/
class PowerDiagramCell_CGAL_2 : public PowerDiagramCell {
public:
    using        Ck                 = CGAL::Exact_predicates_inexact_constructions_kernel;
    using        Rt                 = CGAL::Regular_triangulation_2<Ck>;
    using        Wp                 = typename Rt::Weighted_point;
    using        Pt                 = typename Rt::Point_2;

    using        Vertex             = Rt::Vertex_handle;
    using        Edge               = Rt::Edge;

    void         for_each_edge_point( const std::function<void( const Pt &pt )> &f );
    virtual bool is_infinite        () const override;
    virtual void display            ( VtkOutput &vo, TF *offset = nullptr ) override;
    virtual TF   volume             () override;

    Rt*          rt;                ///<
    Vertex       v;                 ///<
};


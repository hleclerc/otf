#pragma once

//// nsmake avoid_inc CGAL/
//// nsmake lib_name gmp

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Periodic_3_regular_triangulation_3.h>
#include <CGAL/Regular_triangulation_3.h>
#include "PowerDiagramCell.h"

/**
*/
class PowerDiagramCell_CGAL_3 : public PowerDiagramCell {
public:
    using        Ck            = CGAL::Exact_predicates_inexact_constructions_kernel;
    using        Rt            = CGAL::Regular_triangulation_3<Ck>;
    using        Wp            = typename Rt::Weighted_point;
    using        Pt            = typename Rt::Point_3;

    using        Vertex        = Rt::Vertex_handle;
    using        Edge          = Rt::Edge;

    void         for_each_face ( const std::function<void( const Pt *pts_data, PI pts_size )> &f );
    virtual bool is_infinite   () const override;
    virtual void display       ( VtkOutput &vo, TF *offset = nullptr ) override;
    virtual TF   volume        () override;

    Rt*          rt;           ///<
    Vertex       v;            ///<
};


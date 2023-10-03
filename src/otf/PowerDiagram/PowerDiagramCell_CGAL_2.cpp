#include "PowerDiagramCell_CGAL_2.h"
#include "../support/VtkOutput.h"

void PowerDiagramCell_CGAL_2::for_each_edge_point( const std::function<void( const Pt &pt )> &f ) {
    if ( is_infinite() )
        return;

    auto circulator = rt->incident_faces( v ), done( circulator );
    do {
        if ( rt->is_infinite( circulator ) )
            continue;
        Pt dual_orig = rt->weighted_circumcenter( circulator );
        f( dual_orig );
    } while( ++circulator != done );
}

bool PowerDiagramCell_CGAL_2::is_infinite() const {
    auto circulator = rt->incident_faces( v ), done( circulator );
    do {
        if ( rt->is_infinite( circulator ) )
            return true;
    } while( ++circulator != done );
    return false;
}

PowerDiagramCell::TF PowerDiagramCell_CGAL_2::volume() {
    TF res( 0 );
    Pt orig( 0, 0 ), prev( 0, 0 );
    for_each_edge_point( [&]( const Pt &pt ) {
        if ( orig != prev )
            res += Rt::Triangle( orig, prev, pt ).area();
        prev = pt;
    } );
    return res;
}

void PowerDiagramCell_CGAL_2::display( VtkOutput &vo, TF *offset ) {
    Vec<VtkOutput::Pt> pts;
    for_each_edge_point( [&]( const Pt &pt ) {
        pts << VtkOutput::Pt{ pt.x(), pt.y() };
    } );
    vo.add_polygon( pts );
}

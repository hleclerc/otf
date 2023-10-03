#include "PowerDiagramCell_CGAL_2.h"
#include "../support/VtkOutput.h"

void PowerDiagramCell_CGAL_2::for_each_edge_point( const std::function<void( const Pt &pt )> &f ) {
    if ( is_infinite() )
        return;

    // if not boundary, no need to make an intermediate representation
    if ( boundary_offsets->empty() ) {
        auto circulator = rt->incident_faces( v ), done( circulator );
        do {
            if ( rt->is_infinite( circulator ) )
                continue;
            f( rt->weighted_circumcenter( circulator ) );
        } while( ++circulator != done );
        return;
    }

    // else, make a vector with the points
    auto circulator = rt->incident_faces( v ), done( circulator );
    Vec<Pt> pts;
    do {
        if ( rt->is_infinite( circulator ) )
            continue;
        pts << rt->weighted_circumcenter( circulator );
    } while( ++circulator != done );

    // cut with the boundaries
    for( PI n = 0; n < boundary_offsets->size(); ++n )
        cut( pts, { boundary_coeff_x->at( n ), boundary_coeff_y->at( n ) }, boundary_offsets->at( n ) );

    // call f
    for( const Pt &pt : pts )
        f( pt );
}

bool PowerDiagramCell_CGAL_2::is_infinite() const {
    auto circulator = rt->incident_faces( v ), done( circulator );
    do {
        if ( rt->is_infinite( circulator ) )
            return true;
    } while( ++circulator != done );
    return false;
}

void PowerDiagramCell_CGAL_2::cut( Vec<Pt> &pts, Pt dir, TF off ) const {
    Vec<TF> sps = Vec<TF>::from_reservation( pts.size() );
    for( const Pt &pt : pts )
        sps << sp( pt, dir ) - off;

    Vec<Pt> npts;
    for( PI i = 0; i < pts.size(); ++i ) {
        PI j = ( i + 1 ) % pts.size();
        bool ei = sps[ i ] > 0;
        bool ej = sps[ j ] > 0;

        if ( ! ei )
            npts << pts[ i ];

        if ( ei != ej )
            npts << pts[ i ] + sps[ i ] / ( sps[ i ] - sps[ j ] ) * ( pts[ j ] - pts[ i ] );
    }

    pts = std::move( npts );
}

PowerDiagramCell_CGAL_2::TF PowerDiagramCell_CGAL_2::sp( const Pt &a, const Pt &b ) {
    return a.x() * b.x() + a.y() * b.y();
}

PowerDiagramCell_CGAL_2::TF PowerDiagramCell_CGAL_2::n2( const Pt &p ) {
    return p.x() * p.x() + p.y() * p.y();
}

PowerDiagramCell_CGAL_2::TF PowerDiagramCell_CGAL_2::volume() {
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
    Vec<VtkOutput::TF> convex_function;
    for_each_edge_point( [&]( const Pt &pt ) {
        convex_function << sp( pt, v->point().point() ) + ( weight - n2( v->point().point() ) ) / 2;
        pts << VtkOutput::Pt{ pt.x(), pt.y() };
    } );
    vo.add_polygon( pts, { { "convex_function", convex_function } } );
}

#include "PowerDiagramCell_CGAL_2.h"
#include "../support/VtkOutput.h"

void PowerDiagramCell_CGAL_2::for_each_edge_point( const std::function<void( const Pt &pt )> &f ) {
    if ( is_infinite() )
        return;

    auto circulator = rt->incident_faces( v ), done( circulator );
    bool prev_ext = false, first_ext, has_first = false;
    Pt prev_pt, first_pt;
    do {
        if ( rt->is_infinite( circulator ) )
            continue;
        Pt pt = rt->weighted_circumcenter( circulator );
        bool ext = this->ext( pt );
        if ( ! has_first ) {
            has_first = true;
            first_ext = ext;
            first_pt = pt;
        }

//        if ( prev_ext && ! ext )
//            f( inter_bound( pt, prev_pt ) );

        if ( ! ext )
            f( pt );

//        if ( ext && ! prev_ext )
//            f( inter_bound( prev_pt, pt ) );

        prev_ext = ext;
        prev_pt = pt;
    } while( ++circulator != done );

//    if ( first_ext && ! prev_ext )
//        f( inter_bound( first_pt, prev_pt ) );

//    if ( prev_ext && ! first_ext )
//        f( inter_bound( first_pt, prev_pt ) );
}

bool PowerDiagramCell_CGAL_2::is_infinite() const {
    auto circulator = rt->incident_faces( v ), done( circulator );
    do {
        if ( rt->is_infinite( circulator ) )
            return true;
    } while( ++circulator != done );
    return false;
}

PowerDiagramCell_CGAL_2::Pt PowerDiagramCell_CGAL_2::inter_bound( const Pt &inter, const Pt &exter ) const {
    TF best_ext_sp = std::numeric_limits<TF>::max();
    TF int_sp = 0;
    for( PI n = 0; n < boundary_offsets->size(); ++n ) {
        TF ext_sp = sp_bound( exter, n );
        if ( ext_sp > 0 && best_ext_sp > ext_sp ) {
            int_sp = sp_bound( inter, n );
            best_ext_sp = ext_sp;
        }
    }

    return {
        inter.x() + int_sp / ( int_sp - best_ext_sp ) * ( exter.x() - inter.x() ),
        inter.y() + int_sp / ( int_sp - best_ext_sp ) * ( exter.y() - inter.y() )
    };
}

PowerDiagramCell_CGAL_2::TF PowerDiagramCell_CGAL_2::sp_bound( const Pt &p, PI n ) const {
    return boundary_coeff_x->at( n ) * p.x() + boundary_coeff_y->at( n ) * p.y() - boundary_offsets->at( n );
}

bool PowerDiagramCell_CGAL_2::ext( const Pt &p ) const {
    for( PI n = 0; n < boundary_offsets->size(); ++n )
        if ( sp_bound( p, n ) > 0 )
            return true;
    return false;
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

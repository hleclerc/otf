#include "PowerDiagramCell_CGAL_3.h"
#include "../support/VtkOutput.h"
#include <list>

struct PowerDiagramCell_CGAL_3_Inserter {
    using Rt = PowerDiagramCell_CGAL_3::Rt;
    using P = PowerDiagramCell_CGAL_3::Pt;

    PowerDiagramCell_CGAL_3_Inserter &operator++( int ) {
        return *this;
    }

    PowerDiagramCell_CGAL_3_Inserter &operator*() {
        return *this;
    }

    TCT void operator=( const T &edge ) {
        // compute the dual of the edge *eit but handle the translations
        // with respect to the dual of v. That is why we cannot use one
        // of the existing dual functions here.
        Rt::Facet_circulator fstart = rt->incident_facets( edge );
        Rt::Facet_circulator fcit = fstart;
        pts.clear();
        do {
            // TODO: possible speed-up by caching the circumcenters
            P dual_orig = fcit->first->weighted_circumcenter();
            // int idx = fcit->first->index( v );
            // int off = idx;
            pts.push_back( dual_orig );
            ++fcit;
        } while( fcit != fstart );

        f( pts.data(), pts.size() );
    }

    const std::function<void( const P *pts_data, PI pts_size )> &f;
    std::vector<P> pts;
    Rt *rt;
};

void PowerDiagramCell_CGAL_3::get_verts_and_faces( std::vector<double> &v, std::vector<int> &f, std::vector<double> &e ) {
    TODO;
}

void PowerDiagramCell_CGAL_3::for_each_face( const std::function<void( const Pt *pts_data, PI pts_size )> &f ) {
    PowerDiagramCell_CGAL_3_Inserter ins{ f, {}, rt };
    rt->incident_edges( v, ins );
}

bool PowerDiagramCell_CGAL_3::is_infinite() const {
    TODO;
    return false;
}

PowerDiagramCell::TF PowerDiagramCell_CGAL_3::volume() {
    TF res( 0 );
    Pt orig( 0, 0, 0 );
    for_each_face( [&]( const Pt *pts_data, PI pts_size ) {
        for( PI i = 1; i < pts_size - 1; i++ )
            res += Rt::Tetrahedron( orig, pts_data[ 0 ], pts_data[ i ], pts_data[ i + 1 ] ).volume();
    } );
    return res;
}

void PowerDiagramCell_CGAL_3::display( VtkOutput &vo, TF *offset ) {
    for_each_face( [&]( const Pt *pts_data, PI pts_size ) {
        Vec<VtkOutput::Pt> pts;
        for( PI i = 0; i < pts_size; i++ ) {
            VtkOutput::Pt pt{ pts_data[ i ].x(), pts_data[ i ].y(), pts_data[ i ].z() };
            pts << std::move( pt );
        }
        vo.add_polygon( pts );
    } );
}

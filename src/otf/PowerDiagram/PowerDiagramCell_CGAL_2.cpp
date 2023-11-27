#include "PowerDiagramCell_CGAL_2.h"
#include "../support/VtkOutput.h"

using Pt = PowerDiagramCell_CGAL_2::Pt;
using TF = PowerDiagramCell_CGAL_2::TF;

struct FnEdge {
    static void cut( Vec<FnEdge> &edges, Pt dir, TF off ) {
        Vec<FnEdge> new_edges;
        for( const FnEdge &edge : edges ) {
            if ( edge.ray ) {
                TF s = PowerDiagramCell_CGAL_2::sp( edge.b, dir );
                if ( s == 0 ) {
                    if ( PowerDiagramCell_CGAL_2::sp( edge.a, dir ) <= off )
                        new_edges << edge;
                    continue;
                }

                TF r = ( off - PowerDiagramCell_CGAL_2::sp( edge.a, dir ) ) / s;
                if ( ( s > 0 ) ^ ( edge.ray < 0 ) ) {
                    if ( r > 0 ) {
                        new_edges << FnEdge{
                            .a = edge.a,
                            .b = Pt{ edge.a.x() + r * edge.b.x(), edge.a.y() + r * edge.b.y() },
                            .ray = 0
                        };
                    }
                } else {
                    if ( r > 0 ) {
                        new_edges << FnEdge{
                            .a = Pt{ edge.a.x() + r * edge.b.x(), edge.a.y() + r * edge.b.y() },
                            .b = edge.a,
                            .ray = 0
                        };
                    }
                }
                continue;
            }

            // segment
            TF sps_a = PowerDiagramCell_CGAL_2::sp( edge.a, dir ) - off;
            TF sps_b = PowerDiagramCell_CGAL_2::sp( edge.b, dir ) - off;
            bool ea = sps_a > 0;
            bool eb = sps_b > 0;

            // all ext ?
            if ( ea & eb )
                continue;

            // only a is ext ?
            if ( ea && ! eb ) {
                new_edges << FnEdge{
                    .a = edge.a + sps_a / ( sps_a - sps_b ) * ( edge.b - edge.a ),
                    .b = edge.b,
                    .ray = 0
                };
                continue;
            }

            // only b is ext ?
            if ( eb && ! ea ) {
                new_edges << FnEdge{
                    .a = edge.a,
                    .b = edge.a + sps_a / ( sps_a - sps_b ) * ( edge.b - edge.a ),
                    .ray = 0
                };
                continue;
            }

            // => all int
            new_edges << edge;
        }

        std::swap( new_edges, edges );
    }

    Pt a, b;
    int ray;
};

void PowerDiagramCell_CGAL_2::for_each_edge_point( const std::function<void( const Pt &pt )> &f ) {
    // get edges
    auto circulator = rt->incident_edges( v ), done( circulator );
    Vec<FnEdge> edges;
    do {
        CGAL::Object obj = rt->dual( circulator );
        if ( const auto *ray = CGAL::object_cast<Ray>( &obj ) ) {
            if ( rt->is_infinite( circulator ) )
                continue;
            edges << FnEdge{
                .a = Pt{ ray->source().x(), ray->source().y() }, // src
                .b = Pt{ ray->direction().dx(), ray->direction().dy() }, // dir
                .ray = 1
            };
        } else  if ( const auto *seg = CGAL::object_cast<Segment>( &obj ) ) {
            edges << FnEdge{
                .a = seg->point( 0 ),
                .b = seg->point( 1 ),
                .ray = 0
            };
        }
    } while( ++circulator != done );

    // inversion of rays if necessary
    for( PI i = 0; i < edges.size(); ++i ) {
        PI j = ( i + 1 ) % edges.size();
        if ( edges[ i ].ray && edges[ j ].ray ) {
            TF c = 1e1;
            edges[ i ].b = { edges[ i ].a.x() + c * edges[ i ].b.x(), edges[ i ].a.y() + c * edges[ i ].b.y() };
            edges[ j ].b = { edges[ j ].a.x() + c * edges[ j ].b.x(), edges[ j ].a.y() + c * edges[ j ].b.y() };
            std::swap( edges[ j ].a, edges[ j ].b );
            edges[ i ].ray = 0;
            edges[ j ].ray = 0;

            edges.insert( j, FnEdge{ edges[ i ].a, edges[ j ].b, 0 } );

            // edges[ j ].ray = -1;
            break;
        }
    }

    // finite rays
    //    for( FnEdge &edge : edges ) {
    //        if ( edge.ray ) {
    //            TF c = 1e1;
    //            edge.b = { edge.a.x() + c * edge.b.x(), edge.a.y() + c * edge.b.y() };
    //            if ( edge.ray < 0 )
    //                std::swap( edge.a, edge.b );
    //            edge.ray = 0;
    //        }
    //    }

    // cut with the boundaries
    for( PI n = 0; n < boundary_offsets->size(); ++n )
        FnEdge::cut( edges, { boundary_coeff_x->at( n ), boundary_coeff_y->at( n ) }, boundary_offsets->at( n ) );

    // call f
    for( const auto &edge : edges )
        f( edge.a );

    //    // get all the segments
    //    Vec<Vec<Pt,2>> segments;
    //    auto circulator = rt->incident_edges( v ), done( circulator );
    //    do {
    //        CGAL::Object seg = rt->dual( circulator );
    //        if ( const auto *ray = CGAL::object_cast<Ray>( &seg ) ) {
    //            if ( rt->is_infinite( circulator ) )
    //                continue;
    //            // std::cout << "dual: " << ray->source() << "; " << ray->start() << "; " << ray->direction() << std::endl;
    //            Pt r_dir{ ray->direction().dx(), ray->direction().dy() };
    //            Pt r_src{ ray->source().x(), ray->source().y() };

    //            TF best_p = +1e10;
    //            TF best_n = -1e10;
    //            for( PI n = 0; n < boundary_offsets->size(); ++n ) {
    //                Pt b_dir{ boundary_coeff_x->at( n ), boundary_coeff_y->at( n ) };
    //                TF b_off = boundary_offsets->at( n );
    //                TF s = sp( r_dir, b_dir );
    //                if ( s == 0 )
    //                    continue;

    //                TF r = ( b_off - sp( r_src, b_dir ) ) / s;
    //                if ( s > 0 )
    //                    best_p = std::min( best_p, r );
    //                else
    //                    best_n = std::max( best_n, r );
    //            }

    //            segments << Vec<Pt,2>{
    //                best_n <= 0 ? r_src : Pt{ r_src.x() + best_n * r_dir.x(), r_src.y() + best_n * r_dir.y() },
    //                Pt{ r_src.x() + best_p * r_dir.x(), r_src.y() + best_p * r_dir.y() }
    //            };
    //        } else
    //            std::cout << seg.type().name() << std::endl;
    //    } while( ++circulator != done );

    //    if ( segments.empty() )
    //        return;

    //    Vec<Pt> pts;
    //    auto b = segments.pop_back_val();
    //    pts << b[ 0 ] << b[ 1 ];
    //    while ( segments.size() ) {
    //        for( PI i = 0; i < segments.size(); ++i ) {
    //            const Vec<Pt,2> &b = segments[ i ];
    //            if ( b[ 0 ] == pts.back() ) {
    //                pts << b[ 1 ];
    //                segments.remove( i );
    //                break;
    //            }
    //            if ( b[ 1 ] == pts.back() ) {
    //                pts << b[ 0 ];
    //                segments.remove( i );
    //                break;
    //            }
    //            if ( b[ 0 ] == pts[ 0 ] ) {
    //                pts.insert( 0, b[ 1 ] );
    //                segments.remove( i );
    //                break;
    //            }
    //            if ( b[ 1 ] == pts[ 0 ] ) {
    //                pts.insert( 0, b[ 0 ] );
    //                segments.remove( i );
    //                break;
    //            }
    //            assert( 0 );
    //        }
    //    }

    //    for( const Pt &pt : pts )
    //        f( pt );

    //    // if not boundary, no need to make an intermediate representation
    //    if ( boundary_offsets->empty() ) {
    //        auto circulator = rt->incident_faces( v ), done( circulator );
    //        do {
    //            if ( rt->is_infinite( circulator ) ) {
    //                continue;
    //            }
    //            f( rt->weighted_circumcenter( circulator ) );
    //        } while( ++circulator != done );
    //        return;
    //    }

    //    // else, make a vector with the points
    //    auto circulator = rt->incident_faces( v ), done( circulator );
    //    Vec<Pt> pts;
    //    do {
    //        if ( rt->is_infinite( circulator ) ) {
    //            // std::cout << rt->weighted_circumcenter( circulator ) << std::endl;
    //            continue;
    //        }
    //        pts << rt->weighted_circumcenter( circulator );
    //    } while( ++circulator != done );

    //    // cut with the boundaries
    //    for( PI n = 0; n < boundary_offsets->size(); ++n )
    //        cut( pts, { boundary_coeff_x->at( n ), boundary_coeff_y->at( n ) }, boundary_offsets->at( n ) );

    //    // call f
    //    for( const Pt &pt : pts )
    //        f( pt );
}

bool PowerDiagramCell_CGAL_2::is_inside( const Pt &src ) const {
    for( PI n = 0; n < boundary_offsets->size(); ++n )
        if ( sp( src, { boundary_coeff_x->at( n ), boundary_coeff_y->at( n ) } ) > boundary_offsets->at( n ) )
            return false;
    return true;

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

void PowerDiagramCell_CGAL_2::get_verts_and_faces( std::vector<double> &verts, std::vector<int> &faces, std::vector<double> &e ) {
    auto os = verts.size();
    for_each_edge_point( [&]( const Pt &pt ) {
        e.push_back( sp( pt, v->point().point() ) - ( n2( v->point().point() ) - weight ) / 2 );

        verts.push_back( pt.x() );
        verts.push_back( pt.y() );
        verts.push_back( 0 );
    } );
    
    faces.push_back( ( verts.size() - os ) / 3 );
    for( auto i = os; i < verts.size(); i += 3 )
        faces.push_back( i / 3 );
}

void PowerDiagramCell_CGAL_2::display( VtkOutput &vo, TF *offset ) {
    Vec<VtkOutput::Pt> pts;
    Vec<VtkOutput::TF> convex_function;
    for_each_edge_point( [&]( const Pt &pt ) {
        convex_function << sp( pt, v->point().point() ) - ( n2( v->point().point() ) - weight ) / 2;
        pts << VtkOutput::Pt{ pt.x(), pt.y() };
    } );
    vo.add_polygon( pts, { { "convex_function", convex_function } } );
}

#include "PowerDiagramCell_CGAL_3.h"
#include "PowerDiagram_CGAL_3.h"

PowerDiagram_CGAL_3::PowerDiagram_CGAL_3( const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds ) {
    for( PI i = 0; i < nb_points; ++i )
        diracs[ i ] = { { coords[ 0 ][ i ], coords[ 1 ][ i ], coords[ 2 ][ i ] }, weights[ i ] };

    rt = { diracs.begin(), diracs.end() };
}

void PowerDiagram_CGAL_3::for_each_point( const std::function<void( PI num_point, const Vec<PI> &connected_items, PI num_thread )> &f ) {
    TODO;
}

void PowerDiagram_CGAL_3::for_each_cell( const std::function<void( Cell &, PI )> &f ) {
    PowerDiagramCell_CGAL_3 cell;
    cell.rt = &rt;

    PI num_cell = 0;
    for( auto v = rt.finite_vertices_begin(); v != rt.finite_vertices_end(); ++v ) {
        cell.weight = v->point().weight();
        cell.num = num_cell++;
        cell.v = v;

        f( cell, 0 );
    }
}

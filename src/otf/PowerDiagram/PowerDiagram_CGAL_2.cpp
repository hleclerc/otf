#include "PowerDiagramCell_CGAL_2.h"
#include "PowerDiagram_CGAL_2.h"

PowerDiagram_CGAL_2::PowerDiagram_CGAL_2( const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds ) : diracs( nb_points ) {
    for( PI i = 0; i < nb_points; ++i )
        diracs[ i ] = { { coords[ 0 ][ i ], coords[ 1 ][ i ] }, weights[ i ] };

    this->boundary_coeff_x = { boundary_coeffs[ 0 ], boundary_coeffs[ 0 ] + nb_bounds };
    this->boundary_coeff_y = { boundary_coeffs[ 1 ], boundary_coeffs[ 1 ] + nb_bounds };
    this->boundary_offsets = { boundary_offsets, boundary_offsets + nb_bounds };

    rt = { diracs.begin(), diracs.end() };

    PI index = 0;
    for( auto face = rt.all_faces_begin(); face != rt.all_faces_end(); ++face )
        face->info() = index++;
}

void PowerDiagram_CGAL_2::for_each_point( const std::function<void( PI num_point, const Vec<PI> &connected_items, PI num_thread )> &f ) {
    Vec<PI> connected_items;
    for( auto face = rt.all_faces_begin(); face != rt.all_faces_end(); ++face ) {
        connected_items.clear();
        for( PI i = 0; i < 3; ++i )
            connected_items << face->neighbor( i )->info();
        f( face->info(), connected_items, 0 );
    }
}

void PowerDiagram_CGAL_2::for_each_cell( const std::function<void( Cell &, PI )> &f ) {
    PowerDiagramCell_CGAL_2 cell;
    cell.boundary_coeff_x = &boundary_coeff_x;
    cell.boundary_coeff_y = &boundary_coeff_y;
    cell.boundary_offsets = &boundary_offsets;
    cell.rt = &rt;

    PI num_cell = 0;
    for( auto v = rt.all_vertices_begin(); v != rt.all_vertices_end(); ++v ) {
        cell.weight = v->point().weight();
        cell.num = num_cell++;
        cell.v = v;

        f( cell, 0 );
    }
}

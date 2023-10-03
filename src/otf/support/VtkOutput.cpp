#include "VtkOutput.h"
#include <fstream>

void VtkOutput::save( std::string filename ) const {
    std::ofstream os( filename.c_str() );
    save( os );
}

void VtkOutput::save( std::ostream &os ) const {
    os << "# vtk DataFile Version 3.0\n";
    os << "vtk output\n";
    os << "ASCII\n";
    os << "DATASET UNSTRUCTURED_GRID\n";

    // POINTS
    os << "POINTS " << points.size() << " float\n";
    for( const Pt &pt : points )
        os << ( pt.size() >= 1 ? pt[ 0 ] : 0 ) << " " << ( pt.size() >= 2 ? pt[ 1 ] : 0 ) << " " << ( pt.size() >= 3 ? pt[ 2 ] : 0 ) << "\n";

    // CELL
    os << "CELLS " << cell_types.size() << " " << cell_items.size() << "\n";
    for( PI v : cell_items )
        os << v << "\n";

    // CELL_TYPES
    os << "CELL_TYPES " << cell_types.size() << "\n";
    for( PI v : cell_types )
        os << v << "\n";

    // CELL_DATA
    os << "CELL_DATA " << cell_types.size() << "\n";
    os << "FIELD FieldData " << cell_fields.size() << "\n";
    for( const auto &field : cell_fields ) {
        os << field.first << " 1 " << field.second.size() << " float\n";
        for( TF v : field.second )
            os << v << "\n";
    }

    // POINT_DATA
    os << "POINT_DATA " << points.size() << "\n";
    os << "FIELD FieldData " << point_fields.size() << "\n";
    for( const auto &field : point_fields ) {
        os << field.first << " 1 " << field.second.size() << " float\n";
        for( TF v : field.second )
            os << v << "\n";
    }
}

void VtkOutput::add_triangle( std::array<Pt,3> pts ) {
    add_item( pts.data(), pts.size(), 5 );
}

void VtkOutput::add_pyramid( std::array<Pt,5> pts ) {
    std::array<Pt,5> npts{ pts[ 0 ], pts[ 1 ], pts[ 3 ], pts[ 2 ], pts[ 4 ] };
    add_item( npts.data(), npts.size(), 14 );
}

void VtkOutput::add_wedge( std::array<Pt,6> pts ) {
    add_item( pts.data(), pts.size(), 13 );
}

void VtkOutput::add_tetra( std::array<Pt,4> pts ) {
    add_item( pts.data(), pts.size(), 10 );
}

void VtkOutput::add_quad( std::array<Pt,4> pts ) {
    add_item( pts.data(), pts.size(), 9 );
}

void VtkOutput::add_point( Pt pt ) {
    add_item( &pt, 1, VtkPoint );
}

void VtkOutput::add_hexa( std::array<Pt,8> pts ) {
    add_item( pts.data(), pts.size(), 12 );
}

void VtkOutput::add_polygon( const Vec<Pt> &pts, const std::map<String,Vec<TF>> &point_data, const std::map<String,TF> &cell_data ) {
    add_item( pts.ptr(), pts.size(), 7, point_data, cell_data );
}

void VtkOutput::add_line( const Vec<Pt> &pts ) {
    add_item( pts.ptr(), pts.size(), 4 );
}

void VtkOutput::add_item( const Pt *pts_data, PI pts_size, PI vtk_type, const std::map<String,Vec<TF>> &point_data, const std::map<String,TF> &cell_data ) {
    PI old_cell_type_size = cell_types.size();
    PI old_point_size = points.size();

    // geometry
    for( PI i = 0; i < pts_size; ++i )
        points.push_back( pts_data[ i ] );

    cell_items.push_back( pts_size );
    for( PI i = 0, os = old_point_size; i < pts_size; ++i )
        cell_items.push_back( os++ );

    cell_types.push_back( vtk_type );

    // fields
    for( const auto &data : point_data ) {
        auto iter = point_fields.find( data.first );
        if ( iter == point_fields.end() )
            iter = point_fields.insert( iter, { data.first, Vec<TF>::from_size( old_point_size, TF( 0 ) ) } );
        iter->second.append( data.second );
    }
    for( auto &field : point_fields )
        field.second.resize( points.size(), TF( 0 ) );

    for( const auto &data : cell_data ) {
        auto iter = cell_fields.find( data.first );
        if ( iter == cell_fields.end() )
            iter = cell_fields.insert( iter, { data.first, Vec<TF>::from_size( old_cell_type_size, TF( 0 ) ) } );
        iter->second << data.second;
    }
    for( auto &field : cell_fields )
        field.second.resize( cell_types.size(), TF( 0 ) );
}


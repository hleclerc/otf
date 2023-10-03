#include "../../../src/otf/PowerDiagram/PowerDiagramFactory_CGAL.h"
#include "../../../src/otf/support/VtkOutput.h"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <memory>

namespace {
    using TF = PowerDiagram::TF;
    using AF = pybind11::array_t<TF>;

    struct PyPowerDiagram {
        void set_boundary_offsets( AF &that ) {
            boundary_offsets = Vec<TF>::from_function( that.size(), [&]( PI i ) { return that.at( i ); } );

            _pd.clear();
        }

        void set_boundary_coeffs( AF &that ) {
            const PI dim = that.shape( 0 ), n = that.shape( 1 );
            boundary_coeffs.resize( dim );
            for( PI d = 0; d < dim; ++d )
                boundary_coeffs[ d ] = Vec<TF>::from_function( n, [&]( PI i ) { return that.at( d, i ); } );

            _pd.clear();
        }

        void set_positions( AF &that ) {
            const PI dim = that.shape( 0 ), n = that.shape( 1 );
            positions.resize( dim );
            for( PI d = 0; d < dim; ++d )
                positions[ d ] = Vec<TF>::from_function( n, [&]( PI i ) { return that.at( d, i ); } );

            _pd.clear();
        }

        void set_weights( AF &that ) {
            weights = Vec<TF>::from_function( that.size(), [&]( PI i ) { return that.at( i ); } );

            _pd.clear();
        }

        void write_vtk( const String &filename, bool ) {
            VtkOutput vo;
            pd()->for_each_cell( [&]( PowerDiagramCell &cell, PI num_thread ) {
                cell.display( vo );
            } );
            vo.save( filename );
        }

        std::tuple<AF,AF,AF,AF> legendre_transform() {
            //
            pd()->for_each_point( [&]( PI num_point, const Vec<PI> &connected_items, PI num_thread ) {
                std::cout << num_point << ":";
                for( PI ci : connected_items )
                    std::cout << " " << ci;
                std::cout << "\n";
            } );
            return {};
        }

        PowerDiagram *pd() {
            if ( ! _pd ) {
                const PI nb_bounds = boundary_offsets.size();
                const PI nb_points = weights.size();
                const PI dim = positions.size();

                Vec<const TF *> boundary_coeff_ptrs;
                boundary_coeffs.resize( dim );
                for( PI d = 0; d < dim; ++d )
                    boundary_coeff_ptrs << boundary_coeffs[ d ].ptr();

                Vec<const TF *> position_ptrs;
                for( PI d = 0; d < dim; ++d )
                    position_ptrs << positions[ d ].ptr();

                PowerDiagramFactory_CGAL pf;
                _pd = pf.create( dim, position_ptrs.ptr(), weights.ptr(), nb_points, boundary_coeff_ptrs.ptr(), boundary_offsets.ptr(), nb_bounds );
             }

            return _pd.get();
        }

        Vec<TF>          boundary_offsets;
        Vec<Vec<TF>>     boundary_coeffs;
        Vec<Vec<TF>>     positions;
        Vec<TF>          weights;
        Rc<PowerDiagram> _pd;
    };
}

PYBIND11_MODULE( pybind_PowerDiagram, m ) {
    m.doc() = "Power Diagram";

    pybind11::class_<PyPowerDiagram>( m, "PyPowerDiagram" )
        .def( pybind11::init<>(), "" )
        .def( "set_boundary_offsets", &PyPowerDiagram::set_boundary_offsets, "" )
        .def( "set_boundary_coeffs", &PyPowerDiagram::set_boundary_coeffs, "" )
        .def( "set_positions", &PyPowerDiagram::set_positions, "" )
        .def( "set_weights", &PyPowerDiagram::set_weights, "" )

        .def( "legendre_transform", &PyPowerDiagram::legendre_transform, "" )
        .def( "write_vtk", &PyPowerDiagram::write_vtk, pybind11::arg( "filename" ), pybind11::arg( "as_convex_function" ) = false, "" )
    ;
}

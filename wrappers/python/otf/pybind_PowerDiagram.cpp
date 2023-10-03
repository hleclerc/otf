#include "../../../src/otf/PowerDiagram/PowerDiagramFactory_CGAL.h"
#include "../../../src/otf/support/VtkOutput.h"


#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <eigen3/Eigen/LU>
#include <memory>

namespace {
    using TF = PowerDiagram::TF;
    using AF = pybind11::array_t<TF>;

    using TM = Eigen::Matrix<TF,Eigen::Dynamic,Eigen::Dynamic>;
    using TV = Eigen::Matrix<TF,Eigen::Dynamic,1>;

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

        static AF make_AF( const Vec<Vec<TF>> &vec ) {
            AF res;
            if ( vec.size() ) {
                res.resize( { vec.size(), vec[ 0 ].size() } );
                auto buf_res = res.request();
                auto ptr_res = (TF *)buf_res.ptr;

                for( PI r = 0, i = 0; r < vec.size(); ++r )
                    for( PI c = 0; c < vec[ 0 ].size(); ++c, ++i )
                        ptr_res[ i ] = vec[ r ][ c ];
            }
            return res;
        }

        static AF make_AF( const Vec<TF> &vec ) {
            AF res;
            res.resize( { vec.size() } );
            auto buf_res = res.request();
            auto ptr_res = (TF *)buf_res.ptr;

            for( PI r = 0; r < vec.size(); ++r )
                ptr_res[ r ] = vec[ r ];

            return res;
        }

        TF cf_offset( PI n ) {
            TF n2 = 0;
            for( PI c = 0; c < positions.size(); ++c ) {
                TF p = positions[ c ][ n ];
                n2 += p * p;
            }

            return ( n2 - weights[ n ] ) / 2;
        }

        std::tuple<AF,AF,AF,AF> legendre_transform() {
            const PI dim = positions.size();

            Vec<Vec<TF>> new_coeffs = Vec<Vec<TF>>::from_size( dim );
            Vec<TF> new_offsets;
            pd()->for_each_point( [&]( PI, const Vec<PI> &connected_items, PI num_thread ) {
                if ( connected_items.size() != dim + 1 ) {
                    std::cout << "pocsute" << std::endl;
                    return;
                }
                TM M( dim + 1, dim + 1 );
                TV V( dim + 1 );
                for( PI r = 0; r <= dim; ++r ) {
                    for( PI c = 0; c < dim; ++c )
                        M( r, c ) = positions[ c ][ connected_items[ r ] ];
                    M( r, dim ) = -1;

                    V( r ) = cf_offset( connected_items[ r ] );
                }

                Eigen::FullPivLU<TM> lu( M );
                auto X = lu.solve( V );

                for( PI r = 0; r < dim; ++r )
                    new_coeffs[ r ] << X[ r ];
                new_offsets << X[ dim ];
            } );

            Vec<Vec<TF>> new_bound_coeffs = Vec<Vec<TF>>::from_size( dim );
            Vec<TF> new_bound_offsets;

            return { make_AF( new_coeffs ), make_AF( new_offsets ), make_AF( new_bound_coeffs ), make_AF( new_bound_offsets ) };
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

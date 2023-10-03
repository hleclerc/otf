#include "../../../src/otf/PowerDiagram/PowerDiagramFactory_CGAL.h"
#include "../../../src/otf/support/VtkOutput.h"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <memory>

namespace {
    using TF = PowerDiagram::TF;

    // pybind11::array_t<TF> get_integrals( pybind11::array_t<TF> &positions, pybind11::array_t<TF> &weights, Domain &domain, Grid &grid, const FUNC &func ) {
    //     auto ptr_positions = reinterpret_cast<const typename Grid::Pt *>( positions.data() );
    //     auto ptr_weights = reinterpret_cast<const typename Grid::TF *>( weights.data() );

    //     pybind11::array_t<TF> res;
    //     res.resize( { positions.shape( 0 ) } );
    //     auto buf_res = res.request();
    //     auto ptr_res = (TF *)buf_res.ptr;

    //     find_radial_func( func, [&]( const auto &ft ) {
    //         sdot::get_integrals( ptr_res, grid, domain, ptr_positions, ptr_weights, positions.shape( 0 ), ft );
    //     } );

    //     return res;
    // }

    struct PyPowerDiagram {
        PyPowerDiagram( pybind11::array_t<TF> &positions, pybind11::array_t<TF> &weights ) {
            const PI nb_points = weights.size();
            const PI dim = positions.ndim();

            std::vector<const TF *> coords;
            for( PI d = 0; d < dim; ++d )
                coords.push_back( positions.data( d, 0 ) );

            PowerDiagramFactory_CGAL pf;
            pd = pf.create( dim, nb_points, coords.data(), weights.data() );
        }

        void write_vtk( const String &filename ) {
            VtkOutput vo;
            pd->for_each_cell( [&]( PowerDiagramCell &cell, PI num_thread ) {
                cell.display( vo );
            } );
            vo.save( filename );
        }

        Rc<PowerDiagram> pd;
    };
}

PYBIND11_MODULE( pybind_PowerDiagram, m ) {
    m.doc() = "Power Diagram";

    pybind11::class_<PyPowerDiagram>( m, "PyPowerDiagram" )
        .def( pybind11::init<pybind11::array_t<TF> &, pybind11::array_t<TF> &>(), "" )
        .def( "write_vtk", &PyPowerDiagram::write_vtk, "" )
    ;
}

#include "PowerDiagramFactory_CGAL.h"
#include "PowerDiagram_CGAL_2.h"
#include "PowerDiagram_CGAL_3.h"

Rc<PowerDiagram> PowerDiagramFactory_CGAL::create( PI dim, const TF * const *coords, const TF *weights, PI nb_points, const TF * const *boundary_coeffs, const TF *boundary_offsets, PI nb_bounds ) {
    if ( dim == 2 )
        return new PowerDiagram_CGAL_2( coords, weights, nb_points, boundary_coeffs, boundary_offsets, nb_bounds );
    if ( dim == 3 )
        return new PowerDiagram_CGAL_3( coords, weights, nb_points, boundary_coeffs, boundary_offsets, nb_bounds );
    TODO;
}

#include "PowerDiagramFactory_CGAL.h"
#include "PowerDiagram_CGAL_2.h"
#include "PowerDiagram_CGAL_3.h"

Rc<PowerDiagram> PowerDiagramFactory_CGAL::create( PI dim, PI nb_points, const TF * const *coords, const TF *weights ) {
    if ( dim == 2 )
        return new PowerDiagram_CGAL_2( coords, weights, nb_points );
    if ( dim == 3 )
        return new PowerDiagram_CGAL_3( coords, weights, nb_points );
    TODO;
}

#include "../src/otf/PowerDiagram/PowerDiagramFactory_CGAL.h"
#include "../src/otf/support/VtkOutput.h"
#include "catch_main.h"

using TF = PowerDiagram::TF;

void make_pd( PI dim, PI nb_points, const std::function<void( PI dim, PI nb_diracs, const TF **coords, TF *weights )> &f ) {
    std::vector<std::vector<TF>> coord_vecs( dim );
    for( PI d = 0; d < dim; ++d ) {
        coord_vecs[ d ].resize( nb_points );
        for( PI n = 0; n < nb_points; ++n )
            coord_vecs[ d ][ n ] = TF( rand() ) / RAND_MAX;
    }

    std::vector<const TF *> coords( nb_points );
    for( PI d = 0; d < dim; ++d )
        coords[ d ] = coord_vecs[ d ].data();

    std::vector<TF> weights( nb_points );
    for( PI n = 0; n < nb_points; ++n )
        weights[ n ] = 0; // TF( rand() ) / RAND_MAX;

    f( dim, nb_points, coords.data(), weights.data() );
}

TEST_CASE( "Factory", "" ) {
    PowerDiagramFactory_CGAL pf;

    make_pd( 2, 50, [&]( PI dim, PI nb_diracs, const TF **coords, TF *weights ) {
        Rc<PowerDiagram> pd = pf.create( dim, nb_diracs, coords, weights );

        pd->for_each_cell( [&]( PowerDiagramCell &cell, PI ) {
            P( cell.volume() );
        } );

        VtkOutput vo;
        pd->for_each_cell( [&]( PowerDiagramCell &cell, PI ) {
            cell.display( vo );
        } );
        vo.save( "results/pd.vtk" );

        VtkOutput vp;
        for( PI n = 0; n < nb_diracs; ++n )
            vp.add_point( { coords[ 0 ][ n ], coords[ 1 ][ n ] } );
        vp.save( "results/pt.vtk" );
    } );
}

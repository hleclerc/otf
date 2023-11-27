from otf_test_import import ConvexApprox
import numpy as np

bnd_directions = [ [ np.cos( a ), np.sin( a ) ] for a in np.linspace( 0, 2 * np.pi, 4, endpoint = False ) ]
bnd_offsets = np.ones( len( bnd_directions ) ) * 1.5

ca = ConvexApprox.make_approx_from_values_and_derivatives(
    sample_coords = np.random.rand( 2, 150 ) * 2 - 1,
    bnd_directions = bnd_directions,
    bnd_offsets = bnd_offsets,

    f_der = lambda p: [ 2 * p[ 0 ], 2 * p[ 1 ] ],
    f_val = lambda p: p[ 0 ]**2 + p[ 1 ]**2,
)
ca.write_vtk( "test.vtk" )

lt = ca.legendre_transform()
lt.write_vtk( "leta.vtk" )

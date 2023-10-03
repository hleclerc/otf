import test_context
from evtk.hl import pointsToVTK
from otf import ConvexApprox
import numpy as np

def make_approx( f_val, f_der, points, point_file = "" ):
    offsets = []
    coeffs = []
    vals = []
    for num_point in range( points.shape[ 1 ] ):
        point = points[ :, num_point ]
        val = f_val( point )
        der = f_der( point )

        offsets.append( val - np.dot( der, point ) )
        vals.append( list( point ) + [ val ] )
        coeffs.append( der )

    na = 10
    bnd_offsets = np.ones( na )
    bnd_coeffs = [ [ np.cos( a ), np.sin( a ) ] for a in np.linspace( 0, 2 * np.pi, na, endpoint = False ) ]

    if point_file:
        vals = np.array( vals )
        pointsToVTK( point_file,
            np.ascontiguousarray( vals[ :, 0 ] ),
            np.ascontiguousarray( vals[ :, 1 ] ),
            np.ascontiguousarray( vals[ :, 2 ] )
        )

    return ConvexApprox( 
        np.transpose( coeffs ), 
        offsets, 
        np.transpose( bnd_coeffs ),
        bnd_offsets
    )

ca = make_approx(
    lambda p: p[ 0 ]**2 + p[ 1 ]**2,
    lambda p: [ 2 * p[ 0 ], 2 * p[ 1 ] ],
    np.random.rand( 2, 500 ) * 3 - 1.5,
    "points"
)

ca.write_vtk( "test.vtk" )

lt = ca.legendre_transform()
lt.write_vtk( "leta.vtk" )

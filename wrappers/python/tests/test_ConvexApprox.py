import test_context

from otf import ConvexApprox
import numpy as np

def make_approx( f_val, f_der, points ):
    offsets = []
    coeffs = []
    for num_point in range( points.shape[ 1 ] ):
        point = points[ :, num_point ]
        val = f_val( point )
        der = f_der( point )

        offsets.append( val - np.dot( der, point ) )
        coeffs.append( der )

    return ConvexApprox( np.transpose( coeffs ), offsets )

ca = make_approx(
    lambda p: p[ 0 ]**2 + p[ 1 ]**4,
    lambda p: [ 2 * p[ 0 ], 4 * p[ 1 ]**3 ],
    np.random.rand( 2, 100 ) * 2 - 1
)

ca.write_vtk( "test.vtk" )

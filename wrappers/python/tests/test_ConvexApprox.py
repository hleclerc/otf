import test_context

from otf import ConvexApprox
import numpy as np
# import pyvista

def make_approx( f_val, f_der, points ):
    dirs = []
    offs = []
    for num_point in range( points.shape[ 1 ] ):
        point = points[ :, num_point ]
        val = f_val( point )
        der = f_der( point )

        offs.append( np.dot( der, point ) - val )
        dirs.append( der )

    na = 10
    bnd_offsets = np.ones( na ) * 1.5
    bnd_coeffs = [ [ np.cos( a ), np.sin( a ) ] for a in np.linspace( 0, 2 * np.pi, na, endpoint = False ) ]

    return ConvexApprox( 
        np.transpose( dirs ), 
        offs, 
        np.transpose( bnd_coeffs ),
        bnd_offsets
    )

ca = make_approx(
    lambda p: p[ 0 ]**2 + p[ 1 ]**2,
    lambda p: [ 2 * p[ 0 ], 2 * p[ 1 ] ],
    np.random.rand( 2, 500 ) * 2 - 1
)

# pv = ca.power_diagram.as_pyvista_polydata()
# pv.plot()

# ca.write_vtk( "test.vtk" )

# lt = ca.legendre_transform()
# lt.write_vtk( "leta.vtk" )

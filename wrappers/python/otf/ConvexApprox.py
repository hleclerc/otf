from .PowerDiagram import PowerDiagram
import numpy as np


class ConvexApprox:
    def __init__( self, dirs = None, offs = None, boundary_dirs = None, boundary_offs = None ):
        self.power_diagram = PowerDiagram()
        self.sv_dirs = None
        self.sv_offs = None

        self.set_boundary_dirs( boundary_dirs )
        self.set_dirs( dirs )

        self.set_boundary_offs( boundary_offs )
        self.set_offs( offs )

    def set_offs( self, offs ):
        if offs is None:
            return
        self.sv_offs = np.ascontiguousarray( offs )
        self._update_weights()

    def set_dirs( self, dirs ):
        if dirs is None:
            return
        self.sv_dirs = np.ascontiguousarray( dirs )
        self.power_diagram.set_positions( dirs )
        self._update_weights()

    def set_boundary_offs( self, offs ):
        if offs is None:
            return
        self.power_diagram.set_boundary_offsets( offs )

    def set_boundary_dirs( self, dirs ):
        if dirs is None:
            return
        self.power_diagram.set_boundary_directions( dirs )

    def write_vtk( self, filename ):
        self.power_diagram.write_vtk( filename, as_convex_function = True )

    def legendre_transform( self ):
        c = self.power_diagram._inst().legendre_transform()
        return ConvexApprox( *c )

    def _update_weights( self ):
        if self.sv_dirs is None or self.sv_offs is None:
            return
        w = np.sum( self.sv_dirs * self.sv_dirs, axis = 0 ) - 2 * self.sv_offs
        self.power_diagram.set_weights( w )

    def make_approx_from_values_and_derivatives( sample_coords, bnd_directions, bnd_offsets, f_val, f_der ):
        dirs = []
        offs = []
        for num_point in range( sample_coords.shape[ 1 ] ):
            point = sample_coords[ :, num_point ]
            val = f_val( point )
            der = f_der( point )

            offs.append( np.dot( der, point ) - val )
            dirs.append( der )

        return ConvexApprox( 
            np.transpose( dirs ), 
            offs, 
            np.transpose( np.array( bnd_directions ) ),
            np.array( bnd_offsets )
        )

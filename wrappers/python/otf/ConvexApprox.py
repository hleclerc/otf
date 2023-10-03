from .PowerDiagram import PowerDiagram
import numpy as np


class ConvexApprox:
    def __init__( self, coeffs = None, offsets = None, boundary_coeffs = None, boundary_offsets = None ):
        self.power_diagram = PowerDiagram()
        self.sv_offsets = None
        self.sv_coeffs = None

        self.set_boundary_coeffs( boundary_coeffs )
        self.set_coeffs( coeffs )

        self.set_boundary_offsets( boundary_offsets )
        self.set_offsets( offsets )

    def set_offsets( self, offsets ):
        if offsets is None:
            return
        self.sv_offsets = np.ascontiguousarray( offsets )
        self._update_weights()

    def set_coeffs( self, coeffs ):
        if coeffs is None:
            return
        self.sv_coeffs = np.ascontiguousarray( coeffs )
        self.power_diagram.set_positions( coeffs )
        self._update_weights()

    def set_boundary_offsets( self, offsets ):
        if offsets is None:
            return
        self.power_diagram.set_boundary_offsets( offsets )

    def set_boundary_coeffs( self, coeffs ):
        if coeffs is None:
            return
        self.power_diagram.set_boundary_coeffs( coeffs )

    def write_vtk( self, filename ):
        self.power_diagram.write_vtk( filename, as_convex_function = True )

    def _update_weights( self ):
        if self.sv_offsets is None or self.sv_coeffs is None:
            return
        w = 2 * self.sv_offsets + np.sum( self.sv_coeffs * self.sv_coeffs, axis = 0 )
        self.power_diagram.set_weights( w )

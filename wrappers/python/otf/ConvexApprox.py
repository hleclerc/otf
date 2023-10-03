from .PowerDiagram import PowerDiagram
import numpy as np


class ConvexApprox:
    def __init__( self, coeffs = None, offsets = None ):
        self.power_diagram = PowerDiagram()
        self._offsets = None
        self._coeffs = None

        self.set_offsets( offsets )
        self.set_coeffs( coeffs )


    def set_offsets( self, offsets ):
        self._offsets = np.ascontiguousarray( offsets )

        if self._coeffs is not None:
            self._update_weights()

    def set_coeffs( self, coeffs ):
        self._coeffs = np.ascontiguousarray( coeffs )

        self.power_diagram.set_positions( self._coeffs )
        if self._offsets is not None:
            self._update_weights()

    def _update_weights( self ):
        self.power_diagram.set_weights( np.sum( self._coeffs * self._coeffs, axis = 0 ) - self._offsets )

    def write_vtk( self, filename ):
        self.power_diagram.write_vtk( filename )

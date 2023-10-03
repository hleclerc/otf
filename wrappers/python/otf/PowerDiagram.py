import pybind_PowerDiagram
import numpy as np

class PowerDiagram:
    def __init__( self, positions = np.array([[],[]]), weights = np.array([]) ):
        self._cpp_inst = None

    def set_positions( self, positions ):
        self._positions = np.ascontiguousarray( positions )

    def set_weights( self, weights ):
        self._weights = np.ascontiguousarray( weights )

    def write_vtk( self, filename, as_convex_function = False ):
        self._inst().write_vtk( filename, as_convex_function )

    def _inst( self ):
        if self._cpp_inst is None:
            import pybind_PowerDiagram
            self._cpp_inst = pybind_PowerDiagram.PyPowerDiagram( self._positions, self._weights )
        return self._cpp_inst

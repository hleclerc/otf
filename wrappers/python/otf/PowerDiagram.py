import numpy as np

class PowerDiagram:
    def __init__( self, positions = None, weights = None, boundary_directions = None, boundary_offsets = None ):
        self._cpp_inst = None

        self.set_boundary_directions( boundary_directions )
        self.set_positions( positions )

        self.set_boundary_offsets( boundary_offsets )
        self.set_weights( weights )

    def set_boundary_offsets( self, offsets ):
        if offsets is None:
            return
        self._inst().set_boundary_offsets( np.ascontiguousarray( offsets ) )

    def set_boundary_directions( self, coeffs ):
        if coeffs is None:
            return
        self._inst().set_boundary_coeffs( np.ascontiguousarray( coeffs ) )

    def set_positions( self, positions ):
        if positions is None:
            return
        self._inst().set_positions( np.ascontiguousarray( positions ) )

    def set_weights( self, weights ):
        if weights is None:
            return
        self._inst().set_weights( np.ascontiguousarray( weights ) )

    def write_vtk( self, filename, as_convex_function = False ):
        self._inst().write_vtk( filename, as_convex_function )

    def as_pyvista_polydata( self, as_convex_function = False ):
        import pyvista

        ( v, f, e ) = self._inst().vtk_verts_and_faces( as_convex_function )

        res = pyvista.PolyData( np.reshape( v, [ -1, 3 ] ), np.array( f ) )
        res.point_data['convex_function'] = np.array( e )
        return res 


    def _inst( self ):
        if self._cpp_inst is None:
            import pybind_PowerDiagram
            self._cpp_inst = pybind_PowerDiagram.PyPowerDiagram()
        return self._cpp_inst

import test_context
from otf import PowerDiagram
import numpy as np

nb_diracs = 10
points = np.rand( 2, nb_diracs )
pots = np.rand( nb_diracs )

pd = PowerDiagram( points, pots )


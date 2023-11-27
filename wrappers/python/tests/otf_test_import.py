# paths
import sys, os

base_dir = os.path.dirname( os.path.dirname( os.path.abspath( __file__ ) ) )

sys.path.append( base_dir )

base_build = base_dir + "/build"
for dir in os.listdir( base_build ):
    if dir.startswith( 'temp.' ):
        continue
    sys.path.append( base_build + "/" + dir )
    # print( base_build + "/" + dir )

# exports
from otf import ConvexApprox

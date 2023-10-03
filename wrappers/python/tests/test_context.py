import sys, os

base_dir = os.path.dirname( os.path.dirname( os.path.abspath( __file__ ) ) )

sys.path.append( base_dir )
sys.path.append( base_dir + "/build/local/lib/python3.11/dist-packages/otf-0.1.1-py3.11-linux-x86_64.egg/" )


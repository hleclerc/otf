NSMAKE = nsmake --cpp-flag -std=c++20
# NSMAKE += -O3
NSMAKE += -g3

OUT = build/tl

PRG = tests/test_PowerDiagram.cpp
ARG = 

all:
	cd wrappers/python; python tests/test_ConvexApprox.py
	# ${NSMAKE} exe -o ${OUT} ${PRG}
	# ${OUT} ${ARG}

python_build:
	cd wrappers/python; rm -rf build
	cd wrappers/python; python setup.py build
	cd wrappers/python; python setup.py install --prefix=`pwd`/build

vg:
	${NSMAKE} --exec-with "valgrind --num-callers=128 --leak-check=full" run ${PRG} ${ARG}

gdb:
	${NSMAKE} --exec-with "gdb run --args" run ${PRG} ${ARG}

clean:
	nsmake stop
	rm -rf ~/.nsmake
	
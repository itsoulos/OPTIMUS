METHOD=Multistart
PROBLEM=$1
NATOMS=$2
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --threads=2 --localsearch_rate=0.05 --natoms=$NATOMS

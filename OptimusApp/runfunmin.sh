METHOD=Genetic
PROBLEM=$1
NATOMS=$2
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --threads=12 --localsearch_rate=0.1 --natoms=$NATOMS

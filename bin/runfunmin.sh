METHOD=Minfinder
METHODPARAMS=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.01  --genetic_crossover_type=double --chromosomes=100"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.1 --localsearch_method=bfgs"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=25"
elif [ $METHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=200 --mincenter_centers=100 --mincenter_iterations=100"
fi

PROBLEM=$1
NATOMS=$2
echo ./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=12 --iterations=30
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=1 --iterations=30

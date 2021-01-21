METHOD=MinCenter
METHODPARAMS=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=100 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.1 --localsearch_method=bfgs --genetic_crossover_type=double"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.1 --localsearch_method=bfgs"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=25"
elif [ $METHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=1000 --mincenter_centers=100 --mincenter_iterations=200"
fi

PROBLEM=$1
NATOMS=$2
echo ./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=12 --iterations=30
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=1 --iterations=30

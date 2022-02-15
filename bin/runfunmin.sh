METHOD=iPso
METHODPARAMS=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50"
elif [ $METHOD = "gcrs" ]
then
	METHODPARAMS="--gcrs_samples=25 --gcrs_maxiterations=100000"	
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.00  --genetic_crossover_type=double --chromosomes=25"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_particles=100 --pso_localsearch_rate=0.00 --localsearch_method=bfgs"
elif [ $METHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=100 --ipso_localsearch_rate=0.1 --ipso_stoppingrule=best_fitness --ipso_gradientcheck=true --ipso_inertiatype=0"
elif [ $METHOD = "Price" ]
then
	METHODPARAMS="--price_iterations=10000 --price_newpoint=PRICE --price_stopping=DOUBLEBOX"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=25"
elif [ $METHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=600 --mincenter_centers=100 --mincenter_iterations=100"
elif [ $METHOD = "de" ]
then
	METHODPARAMS="--population_count=30 --max_generations=100"
fi

PROBLEM=$1
NATOMS=$2
echo ./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=12 --iterations=30
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --threads=8 --iterations=30

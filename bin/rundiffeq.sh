METHOD=Genetic
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50"
elif [ $METHOD = "IntegerGenetic" ]
then
	METHODPARAMS="--integer_stoprule=stoponzero"	
elif [ $METHOD = "gcrs" ]
then
	METHODPARAMS="--gcrs_samples=25 --gcrs_maxiterations=100000"	
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.05  --genetic_crossover_type=double --chromosomes=500 --localsearch_method=bfgs --max_generations=2000 --genetic_stoprule=stoponzero"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_particles=500 --pso_localsearch_rate=0.005 --localsearch_method=bfgs --pso_generations=2000"
elif [ $METHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=100 --ipso_maxgenerations=100 --ipso_localsearch_rate=0.05 --ipso_stoppingrule=best_fitness -ipso_gradientcheck=true --ipso_inertiatype=2"
elif [ $METHOD = "Price" ]
then
	METHODPARAMS="--price_iterations=10000 --price_newpoint=PRICE --price_stopping=DOUBLEBOX"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=25 --localsearch_method=grs"
elif [ $METHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=600 --mincenter_centers=100 --mincenter_iterations=100"
elif [ $METHOD = "gende" ]
then
	METHODPARAMS="--population_count=30 --max_generations=100"
fi

PROBLEM=$1
MPARAMS="--model=rbf --weights=10 --lambda=10 --npoints=50"

./OptimusApp --filename=../PROBLEMS/DiffEq/lib$PROBLEM.so $MPARAMS  --opt_method=$METHOD  $METHODPARAMS --threads=1 --iterations=10

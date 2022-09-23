METHOD=DoubleGenetic
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50 --minfinder_stoprule=stoponzero"
elif [ $METHOD = "IntegerGenetic" ]
then
	METHODPARAMS="--integer_stoprule=stoponzero --integer_localsearchrate=0.01 --localsearch_method=hill --bfgs_debug=yes"	
elif [ $METHOD = "gcrs" ]
then
	METHODPARAMS="--gcrs_samples=25 --gcrs_maxiterations=100000"	
elif [ $METHOD = "DoubleGenetic" ]
then
	METHODPARAMS="--double_stoprule=stoponzero --double_localsearchrate=0.005 --localsearch_method=bfgs --bfgs_debug=yes --double_generations=1000"	
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.01  --genetic_crossover_type=double --chromosome_count=500 --localsearch_method=bfgs --max_generations=2000 --genetic_stoprule=stoponzero "
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_particles=500 --pso_localsearch_rate=0.01 --localsearch_method=bfgs --pso_generations=2000 --localsearch_method=bfgs --bfgs_debug=yes"
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
	METHODPARAMS="--population_count=200 --max_generations=200"
fi

PROBLEM=$1
MPARAMS="--model=mlp --weights=10 --lambda=100 --npoints=100"

./OptimusApp --filename=../PROBLEMS/DiffEq/lib$PROBLEM.so $MPARAMS  --opt_method=$METHOD  $METHODPARAMS --threads=1 --iterations=1

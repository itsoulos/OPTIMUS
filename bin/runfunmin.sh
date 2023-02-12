METHOD=parallelPso
METHODPARAMS=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50"
elif [ $METHOD = "gcrs" ]
then
	METHODPARAMS="--gcrs_samples=25 --gcrs_maxiterations=100000"	
elif [ $METHOD = "DoubleGenetic" ]
then
	METHODPARAMS="--double_chromosomes=100 --double_generations=200  --sample_method=uniform --localsearch_method=bfgs --double_localsearchrate=0.00"	
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.01  --genetic_crossover_type=double --chromosomes=500 --localsearch_method=bfgs --generations=2000"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_particles=100 --localsearch_method=bfgs --pso_generations=200 --pso_localsearch_rate=0.00"
elif [ $METHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=200 --ipso_maxgenerations=200 --ipso_localsearch_rate=0.05 --ipso_stoppingrule=best_fitness -ipso_gradientcheck=true --ipso_inertiatype=2"
elif [ $METHOD = "Price" ]
then
	METHODPARAMS="--price_iterations=10000 --price_newpoint=PRICE --price_stopping=DOUBLEBOX"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=uniform --multistart_maxiterations=100"

elif [ $METHOD = "Tmlsl" ]
then
	METHODPARAMS="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=rbf --multistart_maxiterations=100"
elif [ $METHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=600 --mincenter_centers=100 --mincenter_iterations=100"
elif [ $METHOD = "parallelPso" ]
then
	METHODPARAMS="--parallelPsoParticles=40 --subCluster=5 --subClusterEnable=1 --pNumber=1 --parallelPropagateMethod=Nto1 --similarityMaxCount=15"
fi

PROBLEM=$1
NATOMS=$2
echo ./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --iterations=30 --threads=10
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD  --natoms=$NATOMS  $METHODPARAMS --iterations=30 --threads=5

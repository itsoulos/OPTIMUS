BOUNDMETHOD=DoubleGenetic
TRAINMETHOD=ParallelDe
DATAPATH=/home/john/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10
METHODPARAMS=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $BOUNDMETHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=50"
elif [ $BOUNDMETHOD = "IntegerGenetic" ]
then
        METHODPARAMS="--integer_chromosomes=500 --integer_generations=500 --sample_method=uniform --localsearch_method=grs --integer_localsearchrate=0.001"
elif [ $BOUNDMETHOD = "gcrs" ]
then
	METHODPARAMS="--gcrs_samples=25 --gcrs_maxiterations=100000"	
elif [ $BOUNDMETHOD = "DoubleGenetic" ]
then
	METHODPARAMS="--double_chromosomes=500 --localsearch_method=bfgs --double_localsearchrate=0.01 --double_debug=no --double_generations=500"	
elif [ $BOUNDMETHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.01  --genetic_crossover_type=double --chromosomes=500 --localsearch_method=bfgs --generations=2000"
elif [ $BOUNDMETHOD = "Pso" ]
then
	METHODPARAMS="--pso_particles=100 --pso_localsearch_rate=0.00 --localsearch_method=bfgs --sample_method=mlp --rbf_samples=100"
elif [ $BOUNDMETHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=100 --ipso_maxgenerations=100 --ipso_localsearch_rate=0.05 --ipso_stoppingrule=best_fitness -ipso_gradientcheck=true --ipso_inertiatype=4"
elif [ $BOUNDMETHOD = "Price" ]
then
	METHODPARAMS="--price_iterations=10000 --price_newpoint=PRICE --price_stopping=DOUBLEBOX"
elif [ $BOUNDMETHOD = "Multistart" ]
then
	METHODPARAMS="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=rbf --multistart_maxiterations=100"

elif [ $BOUNDMETHOD = "Tmlsl" ]
then
	METHODPARAMS="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=rbf --multistart_maxiterations=100"
elif [ $BOUNDMETHOD = "MinCenter" ]
then
	METHODPARAMS="--mincenter_samples=600 --mincenter_centers=100 --mincenter_iterations=100"
elif [ $BOUNDMETHOD = "de" ]
then
	METHODPARAMS="--population_count=30 --max_generations=100"
fi


METHODPARAMS2=""
### Available local search methods: bfgs, gradient, adam, lbfgs
if [ $TRAINMETHOD = "Minfinder" ]
then
	METHODPARAMS2="--minfinder_samples=50"
elif [ $TRAINMETHOD = "IntegerGenetic" ]
then
        METHODPARAMS2="--integer_chromosomes=500 --integer_generations=500 --sample_method=uniform --localsearch_method=grs --integer_localsearchrate=0.001"
elif [ $TRAINMETHOD = "gcrs" ]
then
	METHODPARAMS2="--gcrs_samples=25 --gcrs_maxiterations=100000"
elif [ $TRAINMETHOD = "DoubleGenetic" ]
then
	METHODPARAMS2="--double_chromosomes=200 --sample_method=uniform --localsearch_method=bfgs --double_localsearchrate=0.005 --double_debug=no"
elif [ $TRAINMETHOD = "Genetic" ]
then
	METHODPARAMS2="--localsearch_rate=0.01  --genetic_crossover_type=double --chromosomes=500 --localsearch_method=bfgs --generations=2000"
elif [ $TRAINMETHOD = "Pso" ]
then
	METHODPARAMS2="--pso_particles=100 --pso_localsearch_rate=0.00 --localsearch_method=bfgs --sample_method=mlp --rbf_samples=100"
elif [ $TRAINMETHOD = "iPso" ]
then
	METHODPARAMS2="--ipso_particles=100 --ipso_maxgenerations=100 --ipso_localsearch_rate=0.05 --ipso_stoppingrule=best_fitness -ipso_gradientcheck=true --ipso_inertiatype=4"
elif [ $TRAINMETHOD = "Price" ]
then
	METHODPARAMS2="--price_iterations=10000 --price_newpoint=PRICE --price_stopping=DOUBLEBOX"
elif [ $TRAINMETHOD = "Multistart" ]
then
	METHODPARAMS2="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=rbf --multistart_maxiterations=100"

elif [ $TRAINMETHOD = "Tmlsl" ]
then
	METHODPARAMS2="--multistart_samples=50 --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=500 --sample_method=rbf --multistart_maxiterations=100"
elif [ $TRAINMETHOD = "MinCenter" ]
then
	METHODPARAMS2="--mincenter_samples=600 --mincenter_centers=100 --mincenter_iterations=100"
elif [ $TRAINMETHOD = "ParallelDe" ]
then
	METHODPARAMS2="--parde_agents=200 --parde_islands=1"
fi

MLPARAMS="--trainFile=$1.train --testFile=$1.test --datapath=$DATAPATH --weights=$NODES "
./NeuralTrain  --boundMethod=$BOUNDMETHOD   $METHODPARAMS --trainMethod=$TRAINMETHOD $METHODPARAMS2 $MLPARAMS --iterations=30

#!/bin/bash
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/SECDATA/
NODES=10

METHOD=NeuralMinimizer
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "DoubleGenetic" ]
then
	METHODPARAMS="--double_chromosomes=200 --double_generations=200  --localsearch_method=bfgs --double_localsearchrate=0.001 --sample_method=mlp --mlp_samples=2000"
elif [ $METHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=100 --ipso_localsearch_rate=0.05 --ipso_stoppingrule=best_fitness --ipso_gradientcheck=true --ipso_inertiatype=1 --localsearch_method=adam"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--max_generations=200 --localsearch_rate=0.00 --localsearch_method=lbfgs"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.01"
elif [ $METHOD = "NeuralMinimizer" ]
then
	METHODPARAMS="--neural_model=neural --neural_trainmethod=lbfgs"
elif [ $METHOD = "pDoubleGenetic" ]
then
	METHODPARAMS="--double_chromosomes=200 --double_generations=200  --localsearch_method=bfgs --double_localsearchrate=0.1 --subCluster=1 --subClusterEnable=1 --centers=200 --parallelPropagateMethod=NtoN --pNumber=5  --rbf_samples=100 --rbf_sampler_weights=10 --mlp_samples=1000 --sample_method=uniform double_stoprule=similarity"
	
elif [ $METHOD = "ParallelDe" ]
then
	METHODPARAMS="--parde_agents=100 --parde_islands=2"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=25"
fi

PROBLEM=RbfTest
NODES=$2

MLPARAMS="--trainName=$DATAPATH/$1.train --testName=$DATAPATH/$1.test --nodes=$NODES --natoms=$2"
echo ./OptimusApp --filename=../PROBLEMS/libfunctionalRbf.so  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --iterations=30
./OptimusApp --filename=../PROBLEMS/libfunctionalRbf.so  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS  --iterations=30


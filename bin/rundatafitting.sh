#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10

METHOD=IntegerGenetic
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.001 --localsearch_method=bfgs --max_generations=200 --chromosome_count=500"
elif [ $METHOD = "IntegerGenetic" ]
then
	METHODPARAMS="--integer_localsearchrate=0.000 --localsearch_method=hill --integer_stoprule=generations"
elif [ $METHOD = "DoubleGenetic" ]
then
	METHODPARAMS="--double_localsearchrate=0.01 --localsearch_method=bfgs"	
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.01"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=25"
elif [ $METHOD = "Search" ]
then
	METHODPARAMS=="--Search_method=3"
elif [ $METHOD = "Bfgs" ]
then
	METHODPARAMS="--localsearch_method=adam"
fi

PROBLEM=datafitting

MLPARAMS="--model=gdf --trainName=$DATAPATH/$1.train --testName=$DATAPATH/$1.test --weights=10"
./OptimusApp --filename=../PROBLEMS/DataFitting/lib$PROBLEM.so  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --threads=1  --iterations=30


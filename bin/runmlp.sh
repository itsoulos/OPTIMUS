#!/bin/bash
DATAPATH=/home/john/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
NODES=10

METHOD=ParallelDe
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.001 --localsearch_method=bfgs --max_generations=200 --chromosome_count=500"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.01"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=25"
elif [ $METHOD = "Search" ]
then
	METHODPARAMS=="--Search_method=3"
elif [ $METHOD = "ParallelDe" ]
then
	METHODPARAMS="--parde_agents=100 --parde_islands=2"
elif [ $METHOD = "Bfgs" ]
then
	METHODPARAMS="--localsearch_method=bfgs"
fi

PROBLEM=nntest
NODES=$2

MLPARAMS="--trainName=$DATAPATH/$1.train --testName=$DATAPATH/$1.test --nodes=$2"
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --interval_generations=50 --threads=2  --iterations=30


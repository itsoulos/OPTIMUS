#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/SECDATA/
NODES=10

METHOD=Multistart
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--localsearch_rate=0.00 --localsearch_method=bfgs"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.01"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=25"
fi

PROBLEM=nntest
NODES=$2

MLPARAMS="--trainName=$DATAPATH/$1.train --testName=$DATAPATH/$1.test --nodes=$NODES"
./OptimusApp --filename=../PROBLEMS/lib$PROBLEM.so  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --interval_generations=50 --threads=12  --iterations=30


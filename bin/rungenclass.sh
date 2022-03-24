#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/SECDATA/
NODES=10

METHOD=Genetic
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--chromosome_count=500 --max_generations=500 --localsearch_method=grs --localsearch_rate=0.01 --localsearch_method=bfgs"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.05"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=25"
fi

PROBLEM=DGenclass/libDGenClass.so

MLPARAMS="--trainName=file://$DATAPATH/$1.train --testName=file://$DATAPATH/$1.test "
echo ./OptimusApp --filename=../PROBLEMS/$PROBLEM  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --threads=12 
./OptimusApp --filename=../PROBLEMS/$PROBLEM  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --threads=4 --iterations=30


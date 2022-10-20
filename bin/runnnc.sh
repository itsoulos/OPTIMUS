#!/bin/bash
DATAPATH=/home/sheridan/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=/home/sheridan/Desktop/ERGASIES/SECDATA/
NODES=10

METHOD=IntegerGenetic
METHODPARAMS=""
if [ $METHOD = "Minfinder" ]
then
	METHODPARAMS="--minfinder_samples=25 --minfinder_sampling=repulsion"
elif [ $METHOD = "Genetic" ]
then
	METHODPARAMS="--chromosome_count=500 --max_generations=2000 --localsearch_rate=0.001 --localsearch_method=grs"
elif [ $METHOD = "Pso" ]
then
	METHODPARAMS="--pso_localsearch_rate=0.01 --pso_particles=500 --pso_generations=2000"
elif [ $METHOD = "iPso" ]
then
	METHODPARAMS="--ipso_particles=500 --ipso_maxgenerations=2000 --ipso_localsearch_rate=0.001 --ipso_stoppingrule=best_fitness -ipso_gradientcheck=true --ipso_inertiatype=0"
elif [ $METHOD = "Multistart" ]
then
	METHODPARAMS=="--multistart_samples=50"
fi

PROBLEM=NNC/libNNC.so
FEATURES=$2

MLPARAMS="--trainName=$DATAPATH/$1.train --testName=$DATAPATH/$1.test --features=$FEATURES"
#echo ./OptimusApp --filename=../PROBLEMS/$PROBLEM  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --threads=12  --iterations=30
 ./OptimusApp --filename=../PROBLEMS/$PROBLEM  --opt_method=$METHOD   $METHODPARAMS  $MLPARAMS --threads=1  --iterations=10

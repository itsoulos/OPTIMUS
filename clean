if [ -z $OPTIMUSPATH ]; 
then
	echo "Variable OPTIMUSPATH SHOULD BE SET "
else
	if qmake -v > /dev/null
	then
		(cd PROBLEMS; for file in *.pro; do  qmake $file; make clean;done)
		(cd PROBLEMS; rm -f lib*so* ) 
		(cd lib; rm -f lib*so* )
		(cd PROBLEMS/DeRBF; qmake ; make clean)
		(cd PROBLEMS/DGenClass; qmake; make clean)
		(cd PROBLEMS/DiffEq; qmake; make clean)
		(cd PROBLEMS/Feature; qmake; make clean)
		(cd PROBLEMS/GenClass; qmake; make clean)
		(cd PROBLEMS/NNC; qmake; make clean)
		(cd PROBLEMS/RbfTest; qmake; make clean)
		(cd PROBLEMS/DataFitting; qmake; make clean)
		methods=("Optimus" "islandDE" "islandMultistart" "Bfgs" "IntegerGenetic" "Genetic" "DoubleGenetic" "MinCenter" "Tmlsl" "GENDE" "GCRS" "DE" "Price" "Multistart" "Pso" "iPso"  "Minfinder" "NeuralMinimizer" "ParallelDe" "parallelPso" "ParallelGenetic" "pDoubleGenetic" )
		for str in ${methods[@]}; do
			(cd src/$str; qmake;  make clean )
			(cd src/$str; rm -f *.so )
			(cd src/$str; rm -f *.so.* )
			(cd src/$str; rm -fr .qtc_clangd )
		done
		(cd bin/OptimusFIT; qmake; make clean)
		(cd bin/NeuralTrain; qmake; make clean)
		(cd bin;qmake; make clean; rm -f OptimusApp)
	else
		echo "qmake is not present;"
	fi
fi

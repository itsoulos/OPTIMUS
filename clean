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
		(cd ParallelDe; qmake; make clean)
		(cd src/Optimus; qmake; make clean;)
		(cd src/GCRS; qmake ; make clean )
		(cd src/Genmin; qmake ; make clean )
		(cd src/IntegerGenetic; qmake ; make clean )
		(cd src/parallelPso; qmake ; make clean )
		(cd src/Price; qmake ; make clean )
		(cd src/MinCenter; qmake; make clean )
		(cd src/GENDE; qmake; make clean )
		(cd src/Tmlsl; qmake; make clean )
		(cd src/Multistart; qmake; make clean)
		(cd src/IntervalMethod; qmake; make clean)
		(cd src/Genetic; qmake; make clean)
		(cd src/Pso; qmake; make clean)
		(cd src/iPso; qmake; make clean)
		(cd src/ParallelGenetic; qmake; make clean)
		(cd src/DE; qmake ; make clean )
		(cd src/PSearch; qmake; make clean)
		(cd src/Minfinder; qmake; make clean)
		(cd src/Search; qmake; make clean)
		(cd bin/OptimusFIT; qmake; make clean)
		(cd bin/NeuralTrain; qmake; make clean)
		(cd bin;qmake; make clean; rm -f OptimusApp)
	else
		echo "qmake is not present;"
	fi
fi

if [ -z $OPTIMUSPATH ]; 
then
	echo "Variable OPTIMUSPATH SHOULD BE SET "
else
	if qmake -v > /dev/null
	then
		(cd PROBLEMS; for file in *.pro; do  qmake $file; make clean;done)
		(cd Optimus; qmake; make clean)
		(cd GCRS; make ; make clean )
		(cd Genmin; qmake ; make clean )
		(cd Genprice; qmake ; make clean )
		(cd MinCenter; qmake; make clean )
		(cd WMinCenter; qmake; make clean )
		(cd Multistart; qmake; make clean)
		(cd IntervalMethod; qmake; make clean)
		(cd Genetic; qmake; make clean)
		(cd Pso; qmake; make clean)
		(cd Minfinder; qmake; make clean)
		(cd bin; qmake ; make clean)
	else
		echo "qmake is not present;"
	fi
fi

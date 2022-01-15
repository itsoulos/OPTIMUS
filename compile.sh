if [ -z $OPTIMUSPATH ]; 
then
	echo "Variable OPTIMUSPATH SHOULD BE SET "
else
	if qmake -v > /dev/null
	then
		(cd PROBLEMS; for file in *.pro; do  qmake $file; make clean; make; done)
		(cd PROBLEMS/Feature; for file in *.pro; do  qmake $file; make clean; make; done)
		(cd PROBLEMS/GenClass; for file in *.pro; do  qmake $file; make clean; make; done)
		(cd PROBLEMS/NNC; for file in *.pro; do  qmake $file; make clean; make; done)
		(cd PROBLEMS/RbfTest; for file in *.pro; do  qmake $file; make clean; make; done)
		(cd Optimus; qmake; make clean; make install)
		(cd Bfgs; qmake; make clean; make install )
		(cd MinCenter; qmake; make clean; make install )
		(cd Tmlsl; qmake; make clean; make install )
		(cd GENDE; qmake; make clean; make install )
		(cd GCRS; make ; make clean; make install )
		(cd DE; make ; make clean; make install )
		(cd Genmin; qmake; make clean; make install )
		(cd Price; qmake; make clean; make install )
		(cd Multistart; qmake; make clean; make install)
		(cd Genetic; qmake; make clean; make install)
		(cd Pso; qmake; make clean; make install)
		(cd Minfinder; qmake; make clean; make install)
		(cd bin; qmake ; make clean; make)
	else
		echo "qmake is not present;"
	fi
fi

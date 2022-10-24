TEMPLATE=lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -fopt-info-vec-loop 
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper


HEADERS += interval.h datamodel.h dataset.h gdfmodel.h mlpmodel.h nncmodel.h polymodel.h rbfmodel.h rulermodel.h symbol.h cprogram.h derule.h  fparser.hh deprogram.h doublestack.h neuralprogram.h sigprogram.h neuralparser.h rulercprogram.h
SOURCES+= interval.cpp\
	datafitting.cpp datamodel.cpp dataset.cpp gdfmodel.cpp mlpmodel.cpp doublestack.cc  neuralprogram.cc sigprogram.cc \
       	nncmodel.cpp polymodel.cpp rbfmodel.cpp rulermodel.cpp cprogram.cc symbol.cc derule.cc fparser.cc deprogram.cc neuralparser.cc rulercprogram.cpp

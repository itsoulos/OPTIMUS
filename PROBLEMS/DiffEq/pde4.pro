TEMPLATE =lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -fopt-info-vec-loop 
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper
SOURCES =pde4.cc \
    converter.cc \
    cprogram.cc \
    demodel.cpp \
    deprogram.cc \
    derule.cc \
    doublestack.cc \
    fparser.cc \
    gdfmodel.cpp \
    interval.cpp \
    mlpmodel.cpp \
    neuralparser.cc \
    neuralprogram.cc \
    nncmodel.cpp \
    pdefunction.cc \
    polymodel.cpp \
    rbfmodel.cpp \
    rulercprogram.cpp \
    rulermodel.cpp \
    sigprogram.cc

HEADERS += \
    converter.h \
    cprogram.h \
    demodel.h \
    deprogram.h \
    derule.h \
    doublestack.h \
    fparser.hh \
    gdfmodel.h \
    interval.h \
    mlpmodel.h \
    neuralparser.h \
    neuralprogram.h \
    nncmodel.h \
    polymodel.h \
    rbfmodel.h \
    rulercprogram.h \
    rulermodel.h \
    sigprogram.h

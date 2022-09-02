TEMPLATE=lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -fopt-info-vec-loop 
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper

SOURCES+=ode5.cc \
    converter.cc \
    cprogram.cc \
    demodel.cpp \
    doublestack.cc \
    fparser.cc \
    gdfmodel.cpp \
    interval.cpp \
    mlpmodel.cpp \
    neuralparser.cc \
    neuralprogram.cc \
    nncmodel.cpp \
    odefunction.cc \
    program.cc \
    rbfmodel.cpp \
    rule.cc \
    sigprogram.cc \
    symbol.cc

HEADERS += \
    converter.h \
    cprogram.h \
    demodel.h \
    doublestack.h \
    fparser.hh \
    gdfmodel.h \
    interval.h \
    mlpmodel.h \
    neuralparser.h \
    neuralprogram.h \
    nncmodel.h \
    program.h \
    rbfmodel.h \
    rule.h \
    sigprogram.h \
    symbol.h

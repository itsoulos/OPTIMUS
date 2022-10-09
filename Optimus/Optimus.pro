QT -= gui

TEMPLATE = lib
DEFINES += OPTIMUS_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS+=  -lm -fopenmp

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_CXXFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_LFLAGS += -O3
SOURCES += \
    adam.cpp \
    collection.cc \
    converter.cc \
    cprogram.cc \
    dataset.cpp \
    dllproblem.cpp \
    doublestack.cc \
    fparser.cc \
    gradientdescent.cpp \
    grs.cc \
    interval.cpp \
    intervalproblem.cpp \
    kmeans.cpp \
    lbfgs.cpp \
    nelderMead.cpp \
    neuralparser.cc \
    neuralprogram.cc \
    neuralsampler.cpp \
    nncneuralprogram.cpp \
    nncsampler.cpp \
    optimizer.cpp \
    optimus.cpp \
    point.cpp \
    grspopulation.cc \
    problem.cpp \
    problemsampler.cpp \
    program.cc \
    psolocal.cpp \
    rbfsampler.cpp \
    rlsprogram.cc \
    rule.cc \
    sigprogram.cc \
    symbol.cc \
    tolmin.cc \
    brent.cpp \
    cluster.cpp \
    maxwellsampler.cpp \
    triangularsampler.cpp \
    uniformsampler.cpp

HEADERS += \
    Optimus_global.h \
    adam.h \
    collection.h \
    converter.h \
    cprogram.h \
    dataset.h \
    dllproblem.h \
    doublestack.h \
    fparser.hh \
    gradientdescent.h \
    grs.h \
    interval.h \
    intervalproblem.h \
    kmeans.h \
    lbfgs.h \
    nelderMead.h \
    nelderMead_global.h \
    neuralparser.h \
    neuralprogram.h \
    neuralsampler.h \
    nncneuralprogram.h \
    nncsampler.h \
    optimizer.h \
    optimus.h \
    point.h \
    grspopulation.h \
    problem.h \
    problemsampler.h \
    program.h \
    psolocal.h \
    rbfsampler.h \
    rlsprogram.h \
    rule.h \
    sigprogram.h \
    symbol.h \
    tolmin.h \
    brent.h \
    cluster.h \
    maxwellsampler.h \
    triangularsampler.h \
    uniformsampler.h

# Default rules for deployment.
unix {
    target.path = $$(OPTIMUSPATH)/lib
}
!isEmpty(target.path): INSTALLS += target

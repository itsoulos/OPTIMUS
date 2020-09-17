#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T08:13:00
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Feature
TEMPLATE = lib

QMAKE_CXXFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math -fopt-info
QMAKE_CFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

QMAKE_CXXFLAGS_RELEASE += -std=c++11
QMAKE_CFLAGS_RELEASE += -std=c++11

QMAKE_CXXFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math
QMAKE_CFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

DEFINES += FEATURE_LIBRARY

SOURCES += feature.cpp \
    doublestack.cc \
    interval.cpp \
    symbol.cc \
    rule.cc \
    program.cc \
    fpoptimizer.cc \
    fparser.cc \
    cprogram.cc \
    kmeans.cc \
    matrix_functions.cc \
    mapper.cc \
    model.cc \
    problem.cc \
    neural.cc \
    gensolver.cc \
    nnprogram.cc \
    knn.cc \
    rbf_model.cc \
    Rbf.cc \
    tolmin.cc

HEADERS += feature.h\
        feature_global.h \
    doublestack.h \
    interval.h \
    symbol.h \
    rule.h \
    grand.h \
    program.h \
    fptypes.hh \
    fpconfig.hh \
    fparser.hh \
    cprogram.h \
    kmeans.h \
    matrix_functions.h \
    mapper.h \
    model.h \
    problem.h \
    neural.h \
    gensolver.h \
    nnprogram.h \
    knn.h \
    rbf_model.h \
    Rbf.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

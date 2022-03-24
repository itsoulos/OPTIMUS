#-------------------------------------------------
#
# Project created by QtCreator 2016-04-29T19:58:31
#
#-------------------------------------------------

QT       +=  network


TARGET = DGenClass
TEMPLATE = lib

DEFINES += GENCLASS_LIBRARY

QMAKE_CXXFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math -fopt-info
QMAKE_CFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

QMAKE_CXXFLAGS_RELEASE += -std=c++11
QMAKE_CFLAGS_RELEASE += -std=c++11

QMAKE_CXXFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math
QMAKE_CFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

SOURCES += genclass.cpp \
    interval.cpp \
    symbol.cc \
    doublestack.cc \
    fparser.cc \
    fpoptimizer.cc \
    rule.cc \
    program.cc \
    cprogram.cc \
    classprogram.cc \
    qreplytimeout.cpp

HEADERS += \
    interval.h \
    symbol.h \
    doublestack.h \
    fparser.hh \
    fpconfig.hh \
    fptypes.hh \
    rule.h \
    program.h \
    cprogram.h \
    classprogram.h \
    qreplytimeout.h
    LIBS += -ladept
    DEFINES += ADEPT_RECORDING_PAUSABLE ADEPTSTORAGETHREADSAFE

unix {
    target.path = /usr/lib
    INSTALLS += target
}

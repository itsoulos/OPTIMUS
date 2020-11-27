QT -= gui

TEMPLATE = lib
DEFINES += NNC_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math -fopt-info
QMAKE_CFLAGS_RELEASE += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

QMAKE_CXXFLAGS_RELEASE += -std=c++11
QMAKE_CFLAGS_RELEASE += -std=c++11

QMAKE_CXXFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math
QMAKE_CFLAGS += -O4 -fopenmp -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -ffast-math

SOURCES += \
    converter.cc \
    cprogram.cc \
    doublestack.cc \
    fparser.cc \
    neuralparser.cc \
    neuralprogram.cc \
    nnc.cpp \
    nncneuralprogram.cc \
    program.cc \
    rule.cc \
    sigprogram.cc \
    symbol.cc

HEADERS += \
    NNC_global.h \
    converter.h \
    cprogram.h \
    doublestack.h \
    fparser.hh \
    neuralparser.h \
    neuralprogram.h \
    nnc.h \
    nncneuralprogram.h \
    program.h \
    rule.h \
    sigprogram.h \
    symbol.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

QT -= gui
QT +=  network


TEMPLATE = lib
DEFINES += NNC_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    converter.cc \
    cprogram.cc \
    doublestack.cc \
    fparser.cc \
    interval.cpp \
    neuralparser.cc \
    neuralprogram.cc \
    nnc.cpp \
    tolmin.cc \
    nncneuralprogram.cc \
    program.cc \
    rule.cc \
    sigprogram.cc \
    gensolver.cc \
    symbol.cc

HEADERS += \
    NNC_global.h \
    converter.h \
    cprogram.h \
    doublestack.h \
    fparser.hh \
    interval.h \
    neuralparser.h \
    neuralprogram.h \
    nnc.h \
    nncneuralprogram.h \
    program.h \
    rule.h \
    tolmin.h \
    gensolver.h \
    sigprogram.h \
    symbol.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

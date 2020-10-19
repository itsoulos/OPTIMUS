#-------------------------------------------------
#
# Project created by QtCreator 2016-04-29T19:58:31
#
#-------------------------------------------------

QT       +=  network


TARGET = GenClass
TEMPLATE = lib

DEFINES += GENCLASS_LIBRARY

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

unix {
    target.path = /usr/lib
    INSTALLS += target
}

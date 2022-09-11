QT -= gui

TEMPLATE = lib
DEFINES += DATAFITTING_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cprogram.cc \
    datafitting.cpp \
    datamodel.cpp \
    dataset.cpp \
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
    polymodel.cpp \
    rbfmodel.cpp \
    rulercprogram.cpp \
    rulermodel.cpp \
    sigprogram.cc \
    symbol.cc

HEADERS += \
    DataFitting_global.h \
    cprogram.h \
    datafitting.h \
    datamodel.h \
    dataset.h \
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
    sigprogram.h \
    symbol.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

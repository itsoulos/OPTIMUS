QT -= gui

TEMPLATE = lib
DEFINES += RBFTEST_LIBRARY

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

QMAKE_CXXFLAGS_RELEASE += -Ofast -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-vec
QMAKE_CFLAGS_RELEASE += -Ofast -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread 

QMAKE_CXXFLAGS_RELEASE -= -O2 -O1
QMAKE_CFLAGS_RELEASE -= -O2 -O1

QMAKE_CXXFLAGS += -Ofast -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread -fopt-info-vec
QMAKE_CFLAGS += -Ofast -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -pthread 

QMAKE_CXXFLAGS -= -O2 -O1
QMAKE_CFLAGS -= -O2 -O1

QMAKE_LFLAGS_RELEASE += -Ofast
QMAKE_LFLAGS += -Ofast

QMAKE_LFLAGS_RELEASE -= -O1 -O2
QMAKE_LFLAGS += -O1 -O2

DEFINES+=ADEPT_RECORDING_PAUSABLE
LIBS += -ladept

SOURCES += \
    dataset.cpp \
    interval.cpp \
    kmeans.cpp \
    rbf.cpp \
    rbftest.cpp

HEADERS += \
    RbfTest_global.h \
    dataset.h \
    interval.h \
    kmeans.h \
    rbf.h \
    rbftest.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

QT -= gui

TEMPLATE = lib
DEFINES += PSEARCH_LIBRARY

INCLUDEPATH += $$(OPTIMUSPATH)/include/

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

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_CXXFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_LFLAGS += -O3
SOURCES += \
    psearch.cpp

HEADERS += \
    PSearch_global.h \
    psearch.h

# Default rules for deployment.
LIBS += -L$(OPTIMUSPATH)/lib -lOptimus

# Default rules for deployment.
# Default rules for deployment.
unix {
    target.path = $$(OPTIMUSPATH)/lib
}
!isEmpty(target.path): INSTALLS += target

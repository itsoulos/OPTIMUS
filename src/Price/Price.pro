QT -= gui

TEMPLATE = lib
DEFINES += PRICE_LIBRARY
INCLUDEPATH += $$(OPTIMUSPATH)/include/
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
LIBS+=  -lm -fopenmp

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS_RELEASE += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_CXXFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp
QMAKE_CFLAGS += -O3 -march=native -fopenmp -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -pthread -fopt-info-optimized-vec-loop-omp

QMAKE_LFLAGS += -O3
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collection.cc \
    price.cpp

HEADERS += \
    Price_global.h \
    collection.h \
    price.h


LIBS += -L$(OPTIMUSPATH)/lib -lOptimus

# Default rules for deployment.
# Default rules for deployment.
unix {
    target.path = $$(OPTIMUSPATH)/lib
}
!isEmpty(target.path): INSTALLS += target

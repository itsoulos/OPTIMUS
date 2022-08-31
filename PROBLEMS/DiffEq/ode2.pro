TEMPLATE=lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper -fopt-info-vec-loop 
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper 
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native -mno-vzeroupper

SOURCES+=ode2.cc \
    demodel.cpp \
    interval.cpp \
    mlpmodel.cpp \
    odefunction.cc

HEADERS += \
    demodel.h \
    interval.h \
    mlpmodel.h

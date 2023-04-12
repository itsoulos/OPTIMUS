TEMPLATE=lib
QMAKE_CXXFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  -fopt-info-vec-loop 
QMAKE_CFLAGS_RELEASE += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  

QMAKE_CXXFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native  
QMAKE_CFLAGS += -O4 -march=native -unroll-loops -omit-frame-pointer -Winline -unsafe-math-optimizations -mtune=native 

SOURCES+=gkls350.cc \
    interval.cpp

HEADERS += \
    interval.h

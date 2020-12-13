#ifndef GDESCENT_H
#define GDESCENT_H

#include "MGDescent_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>

class MGDescentInterface
{
public:
    virtual ~MGDescentInterface()
    {

    }
};

class MGDESCENT_EXPORT MGDescent :public Optimizer,MGDescentInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int currentStep;
    long points;    
    double min;
    vector<Data> r;
    vector<Data> rOld;
    vector<Data> gradient;
    vector<Data> lM;
    vector<Data> rM;
    vector<double> m;
    FILE *pFile;
    double rate;
    double gama;
    vector<double> learingRate;
    bool checkifClose();
public:
    MGDescent(Problem *p);
    virtual void Solve() {Optimizer::Solve();};
    virtual ~MGDescent();
};

extern "C" MGDESCENT_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // MGDESCENT_H

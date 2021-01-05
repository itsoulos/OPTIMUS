#ifndef SMGDESCENT_H
#define SMGDESCENT_H

#include "SMGDescent_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>

class SMGDescentInterface
{
public:
    virtual ~SMGDescentInterface()
    {

    }
};

class SMGDESCENT_EXPORT SMGDescent :public Optimizer,SMGDescentInterface
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
    vector<int> rejectedPoints;
    vector<Data> gradient;
    vector<double> learingRate;
    Data lM;
    Data rM;
    vector<double> m;
    vector<double> mins;
    FILE *pFile;
    double rate;
    double gama;
    double fluctuation;
    bool checkifClose();
    bool checkifClose2();
    bool criteria_f(vector<int> v,int p);
    void rejectedP();
public:
    SMGDescent(Problem *p);
    virtual void Solve() {Optimizer::Solve();};
    virtual ~SMGDescent();
};


extern "C" SMGDESCENT_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // SMGDescent_H

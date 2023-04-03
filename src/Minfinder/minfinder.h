#ifndef MINFINDER_H
#define MINFINDER_H

#include "Minfinder_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class MinfinderInterace
{
public:
    virtual ~MinfinderInterace()
    {

    }
};

class MINFINDER_EXPORT Minfinder :public Optimizer,MinfinderInterace
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();

	int sumOfSamples;
    vector<Data> minimax,startx,samplex;
    Data         minimay,starty;
    int          bestIndex;
    int          iteration;
    double       mbesty;
    double       RC;
    int          localSearchCount;
    double      x1,x2,stopat,variance,oldBesty;
    bool  checkForGradientCriterion(Data &x);
    Data    getSample();
    double mdelta(double a,double r0,double r);
public:
    Minfinder(Problem *p);
    virtual void Solve();
    virtual ~Minfinder();
};


extern "C" MINFINDER_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // MINFINDER_H

#ifndef PBRENT_H
#define PBRENT_H

#include "pBrent_global.h"
#include <Optimus/optimizer.h>

class pBrentInterface
{
public:
    virtual ~pBrentInterface()
    {

    }
};

class PBRENT_EXPORT pBrent :public Optimizer, pBrentInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int iteration, dimension;
    Data lm, rm, trialx;
    double mbesty,oldBesty,yy;
    //double x1,x2,stopat,variance;
    double a,b,c,m,e,t;
    int pBrent_samples, pBrent_maxiterations,d;
    vector<Data> points;

public:
    pBrent(Problem *p);
    virtual void Solve();
    virtual ~pBrent();
    double glomin(double &x);
    double r8_epsilon();
    double r8_max(double x, double y);
};

extern "C" PBRENT_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PBRENT_H

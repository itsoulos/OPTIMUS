#ifndef TMLSL_H
#define TMLSL_H

#include "Tmlsl_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>

class TmlslInterface
{
public:
    virtual ~TmlslInterface()
    {

    }
};

class TMLSL_EXPORT Tmlsl: public Optimizer,TmlslInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int    iteration;
    double x1,x2,stopat,variance,oldBesty,mbesty;
    vector<Data> minima,sample,start;
    Data sampley;
    Data minimay;
    double maxRc;
    int N,K,lsearch;
    double rc,drc;
public:
    Tmlsl(Problem *p);
    virtual void Solve();
    double getRC();
    virtual ~Tmlsl();
};

extern "C" TMLSL_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // TMLSL_H

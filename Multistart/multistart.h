#ifndef MULTISTART_H
#define MULTISTART_H

#include "Multistart_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>

class MultistartInterface
{
public:
    virtual ~MultistartInterface()
    {

    }
};

class MULTISTART_EXPORT Multistart :public Optimizer,MultistartInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int    iteration;
    Data trialx;
    double mbesty;
    double x1,x2,stopat,variance,oldBesty;
public:
    Multistart(Problem *p);
    virtual void setSettings(QJsonObject settings);
    virtual void Solve();
    virtual ~Multistart();
};

extern "C" MULTISTART_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // MULTISTART_H

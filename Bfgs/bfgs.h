#ifndef BFGS_H
#define BFGS_H

#include "Bfgs_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>

class BfgsInterface
{
public:
    virtual ~BfgsInterface()
    {

    }
};

class BFGS_EXPORT Bfgs :public Optimizer, BfgsInterface
{
private:
    Data initialx;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
public:
    Bfgs(Problem *p);
    virtual ~Bfgs();
};

extern "C" BFGS_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // BFGS_H

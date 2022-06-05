#ifndef PSEARCH_H
#define PSEARCH_H

#include "PSearch_global.h"
#include <Optimus/optimizer.h>



class PSearchInterface
{
public:
    virtual ~PSearchInterface()
    {

    }
};

class PSEARCH_EXPORT PSearch : public Optimizer, PSearchInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int  nsamples;
    vector<Data> points;
    Data bestPerDim;
    Data bestPoint;

public:
    PSearch(Problem *p);
    virtual void Solve();
    virtual ~PSearch();
};

extern "C" PSEARCH_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PSEARCH_H

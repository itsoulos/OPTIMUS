#ifndef GCRS_H
#define GCRS_H

#include <map>
#include "gcrs_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class GcrsInterface
{
public:
    virtual ~GcrsInterface()
    {
    }
};

class GCRS_EXPORT Gcrs : public Optimizer, GcrsInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int iteration;
    Data xmax, xmin, center, x, xk;
    double fmax,fmin, fk, y;
    bool stopIt, flagF;
    int M, dimension, success, posmax, posmin;
    std::map<int, pair <Data, double>> sample,sample2;    
    vector<int> index;
public:
    Gcrs(Problem *p);
    virtual void Solve();
    virtual ~Gcrs();
};

extern "C" GCRS_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // GCRS_H

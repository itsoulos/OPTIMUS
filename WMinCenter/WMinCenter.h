#ifndef WMINCENTER_H
#define WMINCENTER_H

#include "WMinCenter_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <Optimus/wkmeans.h>
#include <Optimus/cluster.h>
#include <Optimus/point.h>

class MinCenterInterface
{
public:
    virtual ~MinCenterInterface()
    {

    }
};

class WMINCENTER_EXPORT WMinCenter : public Optimizer, MinCenterInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    vector<Point> allSamples;
    vector<Point> allmeans;
    vector<Point> nearMeans(vector<Point>);
    vector<Point> meansWithFewPoints();
    WKMeans *wkmeans;
    int centers, iterations, samples, currentIteration, rate1, minK;
    double rate2;
public:
    WMinCenter(Problem *p);
    virtual ~WMinCenter();
};

extern "C" WMINCENTER_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // WMINCENTER_H

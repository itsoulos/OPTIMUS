#ifndef MINCENTER_H
#define MINCENTER_H

#include "MinCenter_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <Optimus/kmeans.h>
#include <Optimus/cluster.h>
#include <Optimus/point.h>

class MinCenterInterface
{
public:
    virtual ~MinCenterInterface()
    {

    }
};

class MINCENTER_EXPORT MinCenter : public Optimizer, MinCenterInterface
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
    KMeans *kmeans;
    int centers, iterations, samples, currentIteration, rate1, minK;
    double rate2;
public:
    MinCenter(Problem *p);
    virtual ~MinCenter();
};

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // MINCENTER_H

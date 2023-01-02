#ifndef DE_H
#define DE_H

#include "de_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <Optimus/kmeans.h>
#include <Optimus/cluster.h>
#include <Optimus/point.h>

class DEInterface
{
public:
    virtual ~DEInterface()
    {

    }
};

class DE_EXPORT DE : public Optimizer, DEInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    vector<Data> population;

    //for the delb method
    vector<Data> ypopulation;
    Data yfitness_array;

    Data lmargin,rmargin, fitness_array;
    int generation;
    int population_count;
    int population_size;
    int a,b,c,x;
    double bestMin;
    int bestMinIndex;
    double crossoverProbability, differentialWeight;
    double parameterW;
    void    getBestWorst(double &fmin,double &fmax);
    void selectAndCrossover();
    int  tournament(int tsize=8);
    int getBestIndex();
    double randMToN(double M, double N);
public:
    DE(Problem *p);
    virtual ~DE();
};

extern "C" DE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // DE_H

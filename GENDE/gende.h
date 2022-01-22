#ifndef GENDE_H
#define GENDE_H

#include "gende_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>


#include <grs/grs.h>
#include <grs/get_options_price.h>

#include <chrono>

class GENDEInterface
{
public:
    virtual ~GENDEInterface()
    {
    }
};

class GENDE_EXPORT GENDE : public Optimizer, GENDEInterface
{
private:
	double sum,newSum;
	int n;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    vector<Data> population;
    Data lmargin, rmargin, fitness_array, bestPoint;
    double x1, x2, stopat, variance, oldMin, bestMin, indexMin, crossoverProbability, differentialWeight;
    int generation, population_count, population_size, bestMinIndex, dimension, it;
    void selectAndCrossover();
    double randMToN(double M, double N);
    Grs *Solv;
    std::chrono::time_point<std::chrono::system_clock> prin, meta;
public:
    GENDE(Problem *p);
    virtual ~GENDE();
};

extern "C" GENDE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // GENDE_H

#ifndef IPSO_H
#define IPSO_H

#include "iPso_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class iPsoInterface
{
public:
    virtual ~iPsoInterface(){

    };
};

class IPSO_EXPORT iPso : public Optimizer, iPsoInterface
{
private:
    bool centerPso;
    double sum,newSum, besty_tmp;
    int n,sumn;
    int countn;
    Data center;
    vector<Data> particle;
    vector<Data> bestParticle;
    vector<Data> velocity;
    Data bestx;
    Data fitness_array;
    Data bestFitness_array;
    vector<Data> minimax;
    int generation, bestIndex;
    double x1, x2, stopat, variance, besty, oldbesty;
    Data lmargin, rmargin;
    double RC;
    int localSearchCount;

    double fitness(Data &x);
    bool checkGradientCriterion(Data &x);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void calcFitnessArray();
    void updateBest();
    void updateCenter();

public:
    iPso(Problem *p);
    virtual void Solve();
    virtual ~iPso();
};

extern "C" IPSO_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // IPSO_H

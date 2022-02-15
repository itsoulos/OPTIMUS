#ifndef NELDERMEAD_H
#define NELDERMEAD_H

#include "nelderMead_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class nelderMeadInterface
{
public:
    virtual ~nelderMeadInterface()
    {
    }
};

class NELDERMEAD_EXPORT nelderMead : public Optimizer, nelderMeadInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    vector<pair<double, Data>> population;
    Data lmargin, rmargin, fitness_array, bestPoint, centerPoint, reflectedPoint, expandedPoint, contractedPoint;
    double x1, x2, stopat, variance, ybestPoint, sum, newSum, alpha, gamma, ro, sigma,
    worst, secondWorst, yreflectedPoint, yexpandedPoint, ycontractedPoint;
    int generation, population_count, population_size, bestMinIndex, n;
    void order();
    void center();
    void reflection();
    void expansion();
    void contraction();
    void contractionB();
    void shrink();

public:
    nelderMead(Problem *p);
    virtual ~nelderMead();
};

extern "C" NELDERMEAD_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // NELDERMEAD_H

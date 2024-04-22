#ifndef PM_H
#define PM_H

#include "PM_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <QRandomGenerator>
class PMInterface
{
public:
    virtual ~PMInterface(){

    };
};

class PM_EXPORT PM : public Optimizer, PMInterface
{
private:
    double sumDE, newSumDE, sumPSO, newSumPSO, sumMULT, newSumMULT, besty_DE_tmp, besty_DE, besty_PSO,
        besty_PSO_tmp, besty_MULT, besty_MULT_tmp, stopat_DE, variance_DE, stopat_PSO, variance_PSO,
        stopat_MULT, variance_MULT,oldbesty,c1, c2, x1_DE, x2_DE, x1_PSO, x2_PSO, x1_MULT, x2_MULT,
        crossoverProbability, differentialWeight,localRate, besty_DE_old, besty_PSO_old, besty_MULT_old;
    int n, population, maxiterations, dimension, iter, similarityCurrentCountMean_DE, similarityCurrentCountMean_PSO,
        similarityCurrentCountMean_MULT, similarityCurrentCountBest_DE, similarityCurrentCountBest_PSO,
        similarityCurrentCountBest_MULT, similarityMaxCount, optimization, bestyall;

    vector<double> center, lmargin, rmargin, bestx_DE, bestx_PSO, bestx_MULT, fitness_arrayDE,
        fitness_arrayPSO, fitness_arrayMULT, bestFitness_arrayPSO, bestxall;

    vector<vector<double>> points, velocity, elements, particles, bestPoints_PSO;

    QString stoppingrule;

    double fitness(Data &x);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void replace(vector<pair<double, Data>> proelements);
    void updateWithDE();
    void updateWithPso();
    void updateWithMultistart();
    double randMToN(double M, double N);
    void best(int pos);
    void bestAll(int pos);

public:
    PM(Problem *p);
    virtual void Solve();
    virtual ~PM();
};

extern "C" PM_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PM_H

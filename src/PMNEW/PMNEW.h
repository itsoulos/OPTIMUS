#ifndef PMNEW_H
#define PM_H

#include "PMNEW_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <QRandomGenerator>
class PMNEWInterface
{
public:
    virtual ~PMNEWInterface(){

    };
};

class PMNEW_EXPORT PMNEW : public Optimizer, PMNEWInterface
{
private:

    vector<double>  besty, besty_old, stopat, variance, x1, x2, sum, oldSum, lmargin, rmargin, bestxAll, bestyPSO,yDB;

    double crossoverProbability, differentialWeight, localRate, c1, c2, bestyAll;

    vector <int> similarityCurrentCountMean, similarityCurrentCountBest;


    int n, populationofcluster, maxiterations, dimension, iter, similarityMaxCount, optimization, subClusters, subClustersEnable, population, propagation;

    vector<vector<Data>> elements, bestPoints_PSO, velocitys;

    vector<Data> fitness_array, bestx, bestxPSO;

    QString stoppingrule;

    std::chrono::time_point<std::chrono::system_clock> before, after;
    vector<int> subCEnable;

    double fitness(Data &x);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void replace(vector<pair<double, Data>> proelements);
    void updateWithDE(int subCluster);
    void updateWithPso(int subCluster);
    void updateWithMultistart(int subcluster);
    void updateWithArmadillo(int subcluster);
    double randMToN(double M, double N);
    void propagate();
    bool checkSubCluster(int subCluster);
    void checkMean();

public:
    PMNEW(Problem *p);
    virtual void Solve();
    virtual ~PMNEW();
};

extern "C" PMNEW_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PMNEW_H

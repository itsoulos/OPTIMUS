#ifndef PARALLELPSO_H
#define PARALLELPSO_H

#include "parallelPso_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>
#include <Optimus/rbfsampler.h>
#include <QRandomGenerator>
class parallelPsoInterface
{
public:
    virtual ~parallelPsoInterface(){

    };
};

class PARALLELPSO_EXPORT parallelPso : public Optimizer, parallelPsoInterface
{
private:
    int generation, parallelPsoParticles, similarityMaxCount, subCluster, parallelPsoGenerations, n, dimension, localSearchCount, subClusterEnable, pardePropagateRate;

    double x1, x2, stopat, variance, besty, oldbesty, parallelPsoC1, parallelPsoC2, psoLocalSearchRate, RC;

    QString parallelPropagateMethod;

    vector<int> bestSubClusterIndex, similarityCurrentCount;

    vector<double> bestx, fitness_array, bestFitness_array, lmargin, rmargin, bestSubClusterValues, similarityBestValue, sum, newSum, MO, newMO, bestF2xInCluster;

    vector<Data> particles, bestParticle, velocitys, minimax, bestParticleInCluster;

    std::chrono::time_point<std::chrono::system_clock> before, after;

    double fitness(Data &x);
    bool checkGradientCriterion(Data &x);
    int subClusterStartPos(int subClusterIndex);
    int subClusterEndPos(int subClusterIndex);
    void propagateSubClusterValues();
    void getBestValue(int &index, double &value);
    void getBestValue(int islandName, int &index, double &value);
    void replaceValueInSubCluster(int subClusterIndex, Data &x, double &y);
    bool checkSubCluster(int subClusterName);

    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();

public:
    parallelPso(Problem *p);
    virtual void Solve();
    virtual ~parallelPso();
};

extern "C" PARALLELPSO_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PARALLELPSO_H

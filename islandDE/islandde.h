#ifndef ISLANDDE_H
#define ISLANDDE_H

#include "islandde_global.h"
#include <Optimus/tolmin.h>
#include <Optimus/optimizer.h>
#include <QRandomGenerator>
class islandDEInterface
{
public:
    virtual ~islandDEInterface()
    {
    }
};

class ISLANDDE_EXPORT islandDE : public Optimizer, islandDEInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int iteration, maxIt, allSamples, ni, dimension, n, stoppingRule, tour;
    Data mbestx, ysample, lmargin, rmargin, bestMax, bestMin, sum, newSum, MO, newMO;
    double mbesty, yOld, crossoverProbability, differentialWeight;
    vector<vector<double>> xsample, fitness_array;
    vector<vector<pair<double, Data>>> allIsland;
    std::chrono::time_point<std::chrono::system_clock> prin, meta;

    vector<vector<pair<double, Data>>> fragmentationToIslands(vector<Data> Samples, vector<double> ySamples, int n);
    void viewAllSamples(vector<vector<pair<double, Data>>> all);
    void singleDE(vector<pair<double, Data>> samples, int islandName, Data &xmin, double &ymin);
    bool checkIsland(int islandName, int newSum);
    void uniformDistribution(int &N, Matrix &xsample, Data &ysample);
    double randMToN(double M, double N);
    int tournament(int N, int NN);

public:
    islandDE(Problem *p);
    virtual void Solve();
    virtual ~islandDE();
};

extern "C" ISLANDDE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // ISLANDDE_H

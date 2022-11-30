#ifndef ISLANDMULTISTART_H
#define ISLANDMULTISTART_H

#include "islandmultistart_global.h"
#include <Optimus/tolmin.h>
#include <Optimus/optimizer.h>
#include <QRandomGenerator>
class islandMultistartInterface
{
public:
    virtual ~islandMultistartInterface()
    {
    }
};

class ISLANDMULTISTART_EXPORT islandMultistart : public Optimizer, islandMultistartInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int iteration, maxIt, allSamples, ni, dimension, n;
    Data mbestx,ysample;
    double mbesty, yOld;
    vector<vector<double>> xsample, yis;
    vector<vector <double>> fitness_array;
    vector<vector<pair<double,Data>>> allIsland;
    vector<double> sum, newSum;
    std::chrono::time_point<std::chrono::system_clock> prin, meta;

    vector<vector<pair<double,Data>>>  fragmentationToIslands(vector<Data> Samples, vector<double>  ySamples, int n);
    void viewAllSamples(vector<vector<pair<double,Data>>> all);
    void singleMultistart(vector<pair<double,Data>> samples, int islandName, Data &xmin, double  &ymin);
    bool checkIsland(int islandName, int newSum);
    void uniformDistribution(int &N,Matrix &xsample,Data &ysample);

public:
    islandMultistart(Problem *p);
    virtual void Solve();
    virtual ~islandMultistart();
};

extern "C" ISLANDMULTISTART_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // ISLANDMULTISTART_H

#ifndef SEARCH_H
#define SEARCH_H

#include "Search_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class SearchInterface
{
public:
    virtual ~SearchInterface()
    {
    }
};

class SEARCH_EXPORT Search : public Optimizer, SearchInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int currentStep;
    long points;
    double min;
	double dmin;
    int imin;
    int Search_points;
    int Search_maxiterations;
    int Search_method;
    int dimension;
    vector<Data> r;
    vector<Data> rOld;
    vector<int> rejectedPoints;
    vector<Data> gradient;
    vector<double> learingRate;
    Data lM;
    Data rM;
    vector<double> m;
    vector<double> mins;
    FILE *pFile;
    double rate;
    double gama;
    double fluctuation;
    bool checkifClose();
    bool checkifClose2();
    bool criteria_f(vector<int> v, int p);
    void rejectedP(unsigned, double);
    void gd();
    void sgd();
    void mbgd();
    void adam();
    double b1, b2, lRate;
    vector<Data> M_0, U_0;

public:
    Search(Problem *p);
    virtual void Solve() { Optimizer::Solve(); };
    virtual ~Search();
};

extern "C" SEARCH_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // Search_H

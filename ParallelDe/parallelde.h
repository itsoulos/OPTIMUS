#ifndef PARALLELDE_H
#define PARALLELDE_H

#include "ParallelDe_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class ParDEInterface
{
public:
    virtual ~ParDEInterface()
    {
    }
};


class PARALLELDE_EXPORT ParallelDe :public Optimizer, ParDEInterface
{
private:
    Matrix population;
    Data   fitness_array;
    double  parde_F,parde_CR;
    int     islands;
    int     generation;
    Data    lmargin,rmargin;
    Data    bestIslandValues;
    vector<int> bestIslandIndex;

    int     tournament(int islandIndex,int tsize=8);

    int     islandStartPos(int islandIndex);
    int     islandEndPos(int islandIndex);

    void    propagateIslandValues();
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
public:
    ParallelDe(Problem *p);
    virtual ~ParallelDe();
};

extern "C" PARALLELDE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PARALLELDE_H

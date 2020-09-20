#ifndef PSO_H
#define PSO_H

#include "Pso_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
class PsoInterface
{
public:
    virtual  ~PsoInterface()
    {

    };
};

class PSO_EXPORT Pso :public Optimizer,PsoInterface
{
private:
    vector<Data> particle;
    vector<Data> bestParticle;
    vector<Data> velocity;
    Data bestx;
    Data fitness_array;
    Data bestFitness_array;
    vector<Data> minimax;
    int generation,bestIndex;
    double x1,x2,stopat,variance,besty,oldbesty;
    Data lmargin,rmargin;
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
public:
    Pso(Problem *p);
    virtual         void Solve();
    virtual ~Pso();
};

extern "C" PSO_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PSO_H

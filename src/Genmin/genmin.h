#ifndef GENMIN_H
#define GENMIN_H
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
#include "Genmin_global.h"
# include "multi_population.h"
# include "rlsprogram.h"
class GenminInterface
{
public:
    virtual ~GenminInterface()
    {

    }
};

class  Genmin : public Optimizer, GenminInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    MultiPopulation *pop;
    RlsProgram *program;
    Data bestx;
    Data initialx;
    double besty,x1,x2,stopat,variance;
    int genome_count,genome_length;
    double selection_rate,mutation_rate;
    int maxGenerations;
    int generation;
    double initialy;
public:
    Genmin(Problem *p);
    virtual ~Genmin();
};

extern "C" GENMIN_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // GENMIN_H

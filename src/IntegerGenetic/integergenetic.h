#ifndef INTEGERGENETIC_H
#define INTEGERGENETIC_H

#include "IntegerGenetic_global.h"

# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
#include <QtCore/qglobal.h>

#if defined(GENETIC_LIBRARY)
#  define GENETIC_EXPORT Q_DECL_EXPORT
#else
#  define GENETIC_EXPORT Q_DECL_IMPORT
#endif
typedef vector<int> IDATA;

class IntegerGeneticInterface
{
public:
    virtual ~IntegerGeneticInterface()
    {

    }
private:
    virtual void calcFitnessArray()=0;
    virtual void select()=0;
    virtual void crossover()=0;
    virtual void mutate()=0;
    virtual void	getTournamentElement(IDATA &x)=0;
    virtual void	tournament(IDATA &p1,IDATA &p2)=0;
public:

};

class INTEGERGENETIC_EXPORT IntegerGenetic : public Optimizer,IntegerGeneticInterface
{
private:
    vector<IDATA> chromosome;
    vector<IDATA> children;
    Data          fitness_array;
    int         generation;
    Data lmargin,rmargin;
    //for termination rule
    double x1,x2,stopat,variance,oldBesty;

    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void    calcFitnessArray();
    void    select();
    void    crossover();
    void    mutate();
    void	getTournamentElement(IDATA &x);
    void	tournament(IDATA &p1,IDATA &p2);
    Data    fromIDATA(IDATA x);
    IDATA   toIDATA(Data x);
    void    randomSearch(int pos);
    int		randomChromosome();
    void    onepointCrossover(IDATA &parent1, IDATA &parent2, IDATA &children1, IDATA &children2);
    QString localMethod;

public:
    IntegerGenetic(Problem *p);
    virtual         void Solve();
    virtual         ~IntegerGenetic();

};
extern "C" INTEGERGENETIC_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // INTEGERGENETIC_H

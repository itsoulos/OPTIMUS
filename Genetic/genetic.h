#ifndef GENETIC_H
#define GENETIC_H

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

class GeneticInterface
{
public:
    virtual ~GeneticInterface()
    {

    }
private:
    virtual void calcFitnessArray()=0;
    virtual void select()=0;
    virtual void crossover()=0;
    virtual void mutate()=0;
    virtual void	getTournamentElement(Data &x)=0;
    virtual void	tournament(Data &p1,Data &p2)=0;
    virtual bool  checkForGradientCriterion(Data &x)=0;
public:

};

class  Genetic :public Optimizer,GeneticInterface
{
private:
    vector<Data> chromosome,children,minimax;
    Data         fitness_array,lmargin,rmargin,parent0,parent1;
    double     fitness(Data &x);
    int iterNumber;
    //for termination rule
    double x1,x2,stopat,variance,oldBesty;
    double RC;
    int localSearchCount;
    int   generation;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void calcFitnessArray();
    void select();
    /** test comment **/
    void crossover();
    void mutate();
    void	getTournamentElement(Data &x);
    void	tournament(Data &p1,Data &p2);
    bool  checkForGradientCriterion(Data &x);
    double  localSearch(Data &x);
public:
    Genetic(Problem *p);
    virtual         void Solve();
    virtual ~Genetic();
};

extern "C" GENETIC_EXPORT Optimizer *createOptimizer(Problem *p);
#endif // GENETIC_H

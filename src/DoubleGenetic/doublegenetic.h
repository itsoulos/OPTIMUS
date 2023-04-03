#ifndef DOUBLEGENETIC_H
#define DOUBLEGENETIC_H

#include "DoubleGenetic_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
class DoubleGeneticInterface
{
public:
    virtual ~DoubleGeneticInterface()
    {

    }
private:
    virtual void calcFitnessArray()=0;
    virtual void select()=0;
    virtual void crossover()=0;
    virtual void mutate()=0;
    virtual void	getTournamentElement(Data &x)=0;
    virtual void	tournament(Data &p1,Data &p2)=0;
public:

};

/**
 * @brief The DoubleGenetic class implements a Genetic Algorithm for
 * global optimization purposes.
 */
class DOUBLEGENETIC_EXPORT DoubleGenetic: public Optimizer, DoubleGeneticInterface
{
private:
    vector<Data> chromosome;
    vector<Data> children;
    Data        fitnessArray;
    int         generation;
    Data lmargin,rmargin;
    //for termination rule
    double x1,x2,stopat,variance,oldBesty;
    void    calcFitnessArray();
    void    select();
    void    crossover();
    void    mutate();
    void	getTournamentElement(Data &x);
    void	tournament(Data &p1,Data &p2);
    int		randomChromosome();

    void    doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2);

    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();


public:
    /**
     * @brief DoubleGenetic the constructor of the class.
     * @param p
     */
    DoubleGenetic(Problem *p);
    /**
     * @brief Solve, perform the steps of the Genetic algorithm.
     */
    virtual         void Solve();
    virtual         ~DoubleGenetic();
};

extern "C" DOUBLEGENETIC_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // DOUBLEGENETIC_H

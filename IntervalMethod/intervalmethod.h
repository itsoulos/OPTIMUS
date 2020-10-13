#ifndef INTERVALMETHOD_H
#define INTERVALMETHOD_H

#include "IntervalMethod_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>


class IntervalInterface
{
public:
    virtual ~IntervalInterface()
    {

    }
private:
    virtual void    calcFitnessArray()=0;
    virtual void    select()=0;
    virtual void    crossover()=0;
    virtual void    mutate()=0;
    virtual void	getTournamentElement(Data &x)=0;
    virtual void	tournament(Data &p1,Data &p2)=0;
};



class INTERVALMETHOD_EXPORT IntervalMethod :public Optimizer,IntervalInterface
{

private:
    vector<Data> chromosome,children,minimax;
    int genome_size;
    int genome_count;
    Data         fitness_array,lmargin,rmargin,parent0,parent1;
    double     fitness(Data &x);
    vector<Data> randNumber;
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
    void crossover();
    void mutate();
    void    printMargin(Data &x);
    void	getTournamentElement(Data &x);
    void	tournament(Data &p1,Data &p2);
public:
    IntervalMethod(Problem *p);
    virtual void Solve();
    virtual ~IntervalMethod();
};

extern "C" INTERVALMETHOD_EXPORT Optimizer *createOptimizer(Problem *p);


#endif // INTERVALMETHOD_H

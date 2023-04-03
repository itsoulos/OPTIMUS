#ifndef PRICE_H
#define PRICE_H

#include "Price_global.h"

# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include "collection.h"
class PriceInterface
{
public:
    virtual ~PriceInterface()
    {

    }
};
class PRICE_EXPORT Price : public Optimizer, PriceInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    //parameters
    int number_of_samples;//default value -1
    int maximum_number_of_iterations;//default value 10000
    QString new_point_method;//default value PRICE other values MINPOINT, NEWPOINT
    QString stopping_rule;//default value PRICE other values DOUBLEBOX

    Data xmin,xmax,center,xk;
    double fmin,fmax;
    int posmax,posmin;
    Collection *sample2;
    Collection *sample;

    //for the double box stopping rule
    double xx1,xx2;
    int iters,miters;
    double oldfmin;
    double stopat;

    void makeSample2();
    void makeSample();
    Data getNewPoint();
    void makeMinMax();
public:
    Price(Problem *p);
    virtual ~Price();
};

extern "C" PRICE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PRICE_H

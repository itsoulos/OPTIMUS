#ifndef GLSEARCH_H
#define GLSEARCH_H

#include "glsearch_global.h"
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
#include <gsl/gsl_multimin.h>

class GLSearchInterface
{
public:
    virtual ~GLSearchInterface()
    {

    }
};

class GLSEARCH_EXPORT GLSearch :public Optimizer,GLSearchInterface
{
private:
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    int iteration;
    int status;
    const gsl_multimin_fdfminimizer_type *T;
    gsl_multimin_fdfminimizer *s;
    gsl_vector *x;
    gsl_multimin_function_fdf my_func;
public:
    GLSearch(Problem *p);
    virtual void Solve();
    virtual ~GLSearch();
};

extern "C" GLSEARCH_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // GLSEARCH_H

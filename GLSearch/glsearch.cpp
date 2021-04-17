#include "glsearch.h"

double my_f (const gsl_vector *v, void *params)
{
    Problem *t=(Problem *)params;
    Data xx;
    xx.resize(t->getDimension());
    for(int i=0;i<xx.size();i++)
    {
        xx[i]=gsl_vector_get(v,i);
    }
    return t->funmin(xx);
}

void my_df (const gsl_vector *v, void *params, gsl_vector *df)
{
    Problem *t=(Problem *)params;
    Data xx;
    Data gg;
    xx.resize(t->getDimension());
    gg.resize(t->getDimension());
     for(int i=0;i<xx.size();i++)
    {
        xx[i]=gsl_vector_get(v,i);
    }
    t->granal(xx,gg);
    for(int i=0;i<xx.size();i++)
    {
        gsl_vector_set(df, i, gg[i]);
    }
}
void my_fdf (const gsl_vector *x, void *params, double *f, gsl_vector *df)
{

  *f = my_f(x, params);
  my_df(x, params, df);
}
GLSearch::GLSearch(Problem *p)
    :Optimizer(p)
{
    addParameter("glsearch_samples","25","Number of glsearch samples");
    addParameter("glsearch_maxiterations","200","Number of maximum iterations");
}

bool GLSearch::terminated()
{
    //int glsearch_maxiterations=params["glsearch_maxiterations"].toString().toInt();
    return (status != GSL_CONTINUE && iteration >= 100);
    return true;
}

void GLSearch::step()
{
    int glsearch_samples=params["glsearch_samples"].toString().toInt();
    ++iteration;
 
    //gsl_multimin_fdfminimizer_conjugate_fr;
    //gsl_multimin_fdfminimizer_vector_bfgs;
 
    status = gsl_multimin_fdfminimizer_iterate (s);
    status = gsl_multimin_test_gradient (s->gradient, 1e-3);
    if (status == GSL_SUCCESS)
      printf ("Minimum found at:\n");
    printf ("%5d %.5f %.5f %10.5f\n", iteration, gsl_vector_get (s->x, 0), gsl_vector_get (s->x, 1), s->f);
}

void GLSearch::init()
{
    my_func.n = myProblem->getDimension();
     Data xx = myProblem->getRandomPoint();
   
    my_func.f = my_f;
    my_func.df = my_df;
    my_func.fdf = my_fdf;
    my_func.params = this->myProblem;
    x = gsl_vector_alloc (myProblem->getDimension());
     for (unsigned int i = 0;i<x->size;i++)
    {
        gsl_vector_set (x, i, xx[i]);
    }
       T = gsl_multimin_fdfminimizer_vector_bfgs2;
    s = gsl_multimin_fdfminimizer_alloc (T, myProblem->getDimension());
    gsl_multimin_fdfminimizer_set (s, &my_func, x, 0.01, 1e-3);
      
}

void GLSearch::done()
{
    printf ("\n\n");
    gsl_multimin_fdfminimizer_free (s);
    gsl_vector_free (x);
}

void GLSearch::Solve()
{
    Optimizer::Solve();
}

GLSearch::~GLSearch()
{

}

extern "C" GLSEARCH_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new GLSearch(p);
}

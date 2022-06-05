#include "rbf.h"
# include <math.h>
double getDistance(Data &x,Data &y)
{
    double sum = 0.0;
    for(int i=0;i<(int)x.size();i++)
        sum+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum);
}

double gaussian(Data &x,Data &c,double s)
{
    double dist  = getDistance(x,c);
    return exp(-(dist * dist)/s);
}

double RBFgetValue(int nodes,Data &w,Data &point)
{
    double sum = 0.0;
    int dim = point.size();

    Data center;
    center.resize(dim);

    //ta kentra einai diastasis dim. Epomenos ta prota dim*nodes
    //apo to w einai ta kentra kai ta ypoloipa nodes einai ta variances
    //kai ta teleutaia nodes einai ta exoterika vari
    for(int i=0;i<nodes;i++)
    {
        for(int j=0;j<dim;j++)
            center[j]=w[i*dim+j];
       double variance = w[nodes*dim+i];
       double outWeight = w[nodes*dim+nodes+i];
       double g = gaussian(point,center,variance);
       sum+=outWeight * g;
    }
    return sum;
}

double   RBFgetGradient(int nodes,Data &w,Data &point)
{
    double sum = 0.0;
    int dim = point.size();
    Data center;
    center.resize(dim);
    for(int i=0;i<nodes;i++)
    {
        for(int j=0;j<dim;j++)
            center[j]=w[i*dim+j];
       double variance = w[nodes*dim+i];
       double outWeight = w[nodes*dim+nodes+i];
       double g = gaussian(point,center,variance);
       double grad = -2.0/variance * getDistance(point,center);
       sum+=outWeight * g*grad;
    }
    return sum;
}

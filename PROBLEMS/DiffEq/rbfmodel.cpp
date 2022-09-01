#include "rbfmodel.h"

RbfModel::RbfModel(int d,int nweights)
{
    dimension = d;
    weight.resize(nweights);
    variance.resize(nweights);
    center.resize(nweights);
    for(int i=0;i<nweights;i++)
        center[i].resize(dimension);
}
int     RbfModel::getNWeights() const
{
    return weight.size();
}

Data    RbfModel::getWeights()
{
    return weight;
}

void    RbfModel::setWeights(Data &w)
{
    weight = w;
}

double  RbfModel::eval(Data &x)
{
    double sum = 0.0;
    for(int i=0;i<(int)weight.size();i++)
    {
        double px = gaussian(x,center[i],variance[i]);
        sum+=weight[i] * px;
    }
    return sum;
}

double  RbfModel::evalDeriv(Data &x,int pos)
{
    double sum = 0.0;
    for(int i=0;i<(int)weight.size();i++)
    {
        double px = gaussianDerivative(x,center[i],variance[i],pos);
        sum = sum + weight[i]*px;
    }
    return sum;
}

double  RbfModel::evalSecondDeriv(Data &x,int pos)
{
    double sum = 0.0;
    for(int i=0;i<(int)weight.size();i++)
    {
        double px = gaussianSecondDerivative(x,center[i],variance[i],pos);
        sum = sum + weight[i]*px;
    }
    return sum;
}

double  RbfModel::getDistance(Data &x,Data &y)
{
    double sum = 0.0;
    for(unsigned int i=0;i<x.size();i++)
        sum+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum);
}

double  RbfModel::gaussian(Data &x,Data &m,double v)
{
    double dist =getDistance(x,m);
    return exp(-dist * dist/v);
}

double  RbfModel::gaussianDerivative(Data &x,Data &m,double v,int pos)
{
    double hx = gaussian(x,m,v);
    return hx * (-2.0/v)*(x[pos]-m[pos]);
}

double  RbfModel::gaussianSecondDerivative(Data &x,Data &m,double v,int pos)
{
    double hx = gaussian(x,m,v);
    double phixx = (-2.0/v)*(x[pos]-m[pos]);
    return hx *(phixx  * phixx +(-2.0/v));
}

void    RbfModel::setVariables(Data &w)
{
    int weights = weight.size();
    center.resize(weights);
    for(int i=0;i<weights;i++)
        center[i].resize(dimension);
    variance.resize(weights);
    weight.resize(weights);
    for(int i=0;i<weights;i++)
    {
        for(int j=0;j<dimension;j++)
        {
            center[i][j]=w[i*dimension+j];
        }
        variance[i]=w[weights * dimension+i];
        weight[i]=w[weights *dimension+weights+i];
    }
}

int RbfModel::getVariablesSize() const
{
    return 2*weight.size()+weight.size()*dimension;
}

RbfModel::~RbfModel()
{

}

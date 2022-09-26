#include "rbfsampler.h"

RbfSampler::RbfSampler(Problem *p,int w)
{
    myProblem = p;
    nweights = w;
    weight.resize(nweights);
    variance.resize(nweights);
    center.resize(nweights);
    for(int i=0;i<nweights;i++)
    {
        center[i].resize(myProblem->getDimension());
    }
}

double  RbfSampler::getDistance(Data &x,Data &y)
{
    double sum = 0.0;
    for(unsigned int i=0;i<x.size();i++)
        sum+=(x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum);
}

double  RbfSampler::gaussian(Data &x,Data &m,double v)
{
    double dist =getDistance(x,m);
    return exp(-dist * dist/v);
}

int     RbfSampler::getNWeights() const
{
    return nweights;
}

Data    RbfSampler::getWeights()
{
    return weight;
}

void    RbfSampler::setWeights(Data &w)
{
    weight = w;
}

double  RbfSampler::eval(Data &x)
{
    double sum = 0.0;
    for(int i=0;i<(int)weight.size();i++)
    {
        double px = gaussian(x,center[i],variance[i]);
        sum+=weight[i] * px;
    }
    return sum;
}

void    RbfSampler::sampleFromProblem(int N)
{
    xpoint.resize(N);
    ypoint.resize(N);
    for(int i=0;i<N;i++)
    {
        xpoint[i]=myProblem->getRandomPoint();
        ypoint[i]=myProblem->funmin(xpoint[i]);
    }
}

void    RbfSampler::trainModel()
{
    //kmeans first
    //then train
}

void    RbfSampler::sampleFromModel(int N,vector<Data> &xsample,Data &ysample)
{
    //sample many points and take the N lowest values
}

RbfSampler::~RbfSampler()
{
    //nothing for now
}

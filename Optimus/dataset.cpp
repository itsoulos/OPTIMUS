#include "dataset.h"

Dataset::Dataset(Matrix &x,Data &y)
{
    xpoint.resize(x.size());
    ypoint.resize(y.size());
    unsigned int i;
    for(i=0;i<x.size();i++)
    {
        xpoint[i].resize(x[i].size());
        xpoint[i] = x[i];
        ypoint[i] = y[i];
    }
}

Dataset::Dataset(Problem *p,int N)
{
    xpoint.resize(N);
    ypoint.resize(N);
    for(int i=0;i<N;i++)
    {
        xpoint[i].resize(p->getDimension());
        xpoint[i]=p->getRandomPoint();
        ypoint[i]=p->funmin(xpoint[i]);
    }
}

Data    Dataset::getx(int pos) const
{
    return xpoint[pos];
}

double  Dataset::gety(int pos) const
{
    return ypoint[pos];
}

int     Dataset::getdimension() const
{
    return xpoint[0].size();
}

int     Dataset::getpatterns() const
{
    return xpoint.size();
}

Dataset::~Dataset()
{
    //nothing for now
}

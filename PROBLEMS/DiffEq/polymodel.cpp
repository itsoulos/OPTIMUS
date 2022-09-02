#include "polymodel.h"

PolyModel::PolyModel(int d,int m)
{
    dimension = d;
    weights.resize(m * dimension);
}

void    PolyModel::setWeights(Data &x)
{
    weights = x;
}

double  PolyModel::eval(Data &x)
{
    double value = 0.0;
    for(int i=0;i<dimension;i++)
    {
        double pol = 0.0;
        int maxbase = weights.size()/dimension;
        for(int j=1;j<=maxbase;j++)
        {
            pol+=weights[i*maxbase+j]*pow(x[i],j);
        }
        value+=pol;
    }
    return value;
}

double  PolyModel::evalDeriv(Data &x,int pos)
{
    double pol = 0.0;
    int maxbase = weights.size()/dimension;
    for(int j=1;j<=maxbase;j++)
    {
        pol+=j * weights[pos*maxbase+j]*pow(x[pos],j-1);
    }
    return pol;
}

double  PolyModel::evalSecondDeriv(Data &x,int pos)
{
    double pol = 0.0;
    int maxbase = weights.size()/dimension;
    for(int j=2;j<=maxbase;j++)
    {
        pol+=j *(j-1) * weights[pos*maxbase+j]*pow(x[pos],j-2);
    }
    return pol;
}

PolyModel::~PolyModel()
{

}

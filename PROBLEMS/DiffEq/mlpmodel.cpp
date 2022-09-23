#include "mlpmodel.h"
MlpModel::MlpModel(int d,int nweights)
{
    dimension = d;
    weight.resize((dimension+2)*nweights);
}

int     MlpModel::getNWeights() const
{
    return weight.size()/(dimension+2);
}

Data    MlpModel::getWeights()
{
    return weight;
}

void    MlpModel::setWeights(Data &w)
{
    weight = w;
}

double  MlpModel::eval(Data &x)
{
    int nodes=weight.size()/ (dimension + 2);
        double per=0.0;
        for(int i=1;i<=nodes;i++)
        {
            double arg=0.0;
            for(int j=1;j<=dimension;j++)
            {
                int pos=(dimension+2)*i-(dimension+1)+j;
                arg+=x[j-1]*weight[pos-1];
            }
            arg+=weight[(dimension+2)*i-1];
            per+=weight[(dimension+2)*i-(dimension+1)-1]*sig(arg);

        }
        return per;
}


double  MlpModel::evalDeriv(Data &x,int pos)
{
    int nodes=weight.size()/ (dimension + 2);
        double per=0.0;
        for(int i=1;i<=nodes;i++)
        {
            double arg=0.0;
            for(int j=1;j<=dimension;j++)
            {
                int mypos=(dimension+2)*i-(dimension+1)+j;
                arg+=x[j-1]*weight[mypos-1];
            }
            arg+=weight[(dimension+2)*i-1];
            double s=sig(arg);
//            per+=weight[(dimension+2)*i-(dimension+1)-1]*s*(1.0-s)*weight[(dimension+2)*i-(dimension+1)+pos-1];
            per+=weight[(dimension+2)*i-(dimension+1)-1]*s*(1.0-s)*weight[(dimension+2)*i-(dimension+1)+pos];

        }
        return per;
}

double  MlpModel::evalSecondDeriv(Data &x,int pos)
{
    int nodes=weight.size()/ (dimension + 2);
        double per=0.0;
        for(int i=1;i<=nodes;i++)
        {
            double arg=0.0;
            for(int j=1;j<=dimension;j++)
            {
                int mypos=(dimension+2)*i-(dimension+1)+j;
                arg+=x[j-1]*weight[mypos-1];
            }
            arg+=weight[(dimension+2)*i-1];
            double s=sig(arg);
            double w1=weight[(dimension+2)*i-(dimension+1)-1];
//            double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
            double w2=weight[(dimension+2)*i-(dimension+1)+pos];

            per+=w1*w2*w2*s*(1.0-s)*(1.0-2*s);
        }
        return per;
}

void   MlpModel::getModelDeriv(Data &xpoint,Data &g)
{

    for(int i=0;i<g.size();i++) g[i]=0.0;
    Data tempg;
    Data tempg2;
    tempg.resize(g.size());
    tempg2.resize(g.size());

    int nodes=weight.size()/ (dimension + 2);
    for(int i=1;i<=nodes;i++)
    {
        double arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j;
            arg+=xpoint[j-1]*weight[pos-1];
        }
        arg+=weight[(dimension+2)*i-1];
        double s=sig(arg);
        double s2=sigder(arg);
        g[(dimension+2)*i-(dimension+1)-1]=s;
        g[(dimension+2)*i-1]=weight[(dimension+2)*i-(dimension+1)-1]*s2;
        for(int j=1;j<=dimension;j++)
        {
            int pos=(dimension+2)*i-(dimension+1)+j;
            g[pos-1]=weight[(dimension+2)*i-(dimension+1)-1]*xpoint[j-1]*s2;
        }
    }

}

void	MlpModel::getXDeriv(vector<double> xpoint,int pos,vector<double> &g)
{
    pos = pos +1;
    int nodes=weight.size()/ (dimension + 2);
    for(int i=1;i<=nodes;i++)
    {
        double arg=0.0;
        for(int j=1;j<=dimension;j++)
        {
            int mypos=(dimension+2)*i-(dimension+1)+j;
            arg+=xpoint[j-1]*weight[mypos-1];
        }
        arg+=weight[(dimension+2)*i-1];
        double s=sig(arg);
        double w1=weight[(dimension+2)*i-(dimension+1)-1];
        double w2=weight[(dimension+2)*i-(dimension+1)+pos-1];
        double sigder=s*(1-s);
        double sigder2=s*(1-s)*(1-2.0*s);
        double sigder3=sigder*(6*s*s-6*s+1);
        g[(dimension+2)*i-1]=w1*w2*sigder2;
        g[(dimension+2)*i-(dimension+1)-1]=w2*sigder;
        for(int j=1;j<=dimension;j++)
        {
            int mypos=(dimension+2)*i-(dimension+1)+j;
            if(j!=pos)
            g[mypos-1]=w1*w2*xpoint[j-1]*sigder2;
            else
            g[mypos-1]=w1*sigder+w1*w2*xpoint[j-1]*sigder2;
        }
    }
}

MlpModel::~MlpModel()
{

}

double  MlpModel::sig(double x)
{
    return 1.0/(1.0+exp(-x));
}

double MlpModel::sigder(double x)
{
    double s = sig(x);
    return s*(1.0-s);
}

double MlpModel::sigder2(double x)
{
    double s = sigder(x);
    return s * (1.0 -s);
}

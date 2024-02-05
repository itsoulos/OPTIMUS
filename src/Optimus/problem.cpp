#include "problem.h"
Problem::Problem(IntervalProblem *p)
{
    problem=p;
    dimension=p->getDimension();
    lmargin.resize(dimension);
    rmargin.resize(dimension);
    IntervalData bound=p->getMargins();
    for(int i=0;i<dimension;i++)
    {
        lmargin[i]=bound[i].leftValue();
        rmargin[i]=bound[i].rightValue();
    }
    functionCalls=0;
    tempx.resize(dimension);
    tempg.resize(dimension);
    bestx.resize(dimension);
    besty=1e+100;
}

int	Problem::getFunctionCalls()
{
	return functionCalls;
}

double Problem::randomElement(int pos)
{
    double r = randomDouble();
    return lmargin[pos]+r * (rmargin[pos]-lmargin[pos]);
}

Data   Problem::getBestx()
{
    return bestx;
}

double  Problem::getBesty()
{
    return besty;
}

double getDistance(Data &x1,Data &x2)
{
    double sum=0.0;
    for(int i=0;i<x1.size();i++)
        sum+=(x1[i]-x2[i])*(x1[i]-x2[i]);
    return sqrt(sum);
}

void	Problem::resetFunctionCalls()
{
	functionCalls=0;

    besty=1e+100;
}

void    Problem::resetFunctionCalls(QJsonObject &settings)
{
    resetFunctionCalls();
    problem->init(settings);
}

Data       Problem::getUniformRandomPoint()
{
    Data x;
    x.resize(dimension);
    for(int i=0;i<dimension;i++)
    {
        double r=randomDouble();
        x[i]=(2.0*r-1.0);
    }
    return x;
}

Problem::Problem(IntervalProblem *p,IntervalData &m)
{
    problem=p;
    dimension=p->getDimension();
    lmargin.resize(dimension);
    rmargin.resize(dimension);

    for(int i=0;i<dimension;i++)
    {
        lmargin[i]=m[i].leftValue();
        rmargin[i]=m[i].rightValue();
    }
}

int  Problem::getDimension() const
{
    return dimension;
}

Data Problem::getLeftMargin() const
{
    return lmargin;
}

Data Problem::getRightMargin() const
{
    return rmargin;
}

double  Problem::randomDouble()
{
    return problem->randomDouble();
}
void       Problem::getleftmarginx(double *x)
{
   for(int i=0;i<lmargin.size();i++)
       x[i]=lmargin[i];
}
void       Problem::getrightmarginx(double *x)
{
    for(int i=0;i<lmargin.size();i++)
        x[i]=rmargin[i];
}
double Problem::funmin(Data &x)
{
    double y;
	++functionCalls;
#pragma omp critical
    {
    y=problem->funmin(x);
    if(y<besty)
    {
        besty=y;
        bestx=x;
    }
}
    return y;
}


double  Problem::funmin(double *x)
{
    for(int i=0;i<tempx.size();i++)
        tempx[i]=x[i];
    return funmin(tempx);
}


void Problem::granal(double *x, double *g)
{
    for(int i=0;i<tempx.size();i++)
        tempx[i]=x[i];
    granal(tempx,tempg);
    for(int i=0;i<tempx.size();i++)
        g[i]=tempg[i];
}
void Problem::granal(Data &x,Data &g)
{
    problem->granal(x,g);
}

Data    Problem::getRandomPoint()
{

    Data x;
    x.resize(dimension);

    problem->getSample(x);
    /*
    for(int i=0;i<dimension;i++)
    {
        double r=randomDouble();
        //double middle=lmargin[i]+(rmargin[i]-lmargin[i])/2.0;
        //double delta = (r-1.0);
        //x[i]=middle+delta*(0.1 * (rmargin[i]-lmargin[i]));
        x[i]=lmargin[i]+r*(rmargin[i]-lmargin[i]);
    }*/
    return x;
}


bool    Problem::isPointIn(Data &x)
{
    for(int i=0;i<x.size();i++)
        if(x[i]<lmargin[i] || x[i]>rmargin[i])
            return false;
    return true;
}


bool     Problem::getGradientCriterion(Data &x1,Data &x2)
{
        Data g1,g2;
        g1.resize(x1.size());
        g2.resize(x2.size());
        granal(x1,g1);
        granal(x2,g2);
        double s=0.0;
        for(int i=0;i<x1.size();i++)
                s+=(x1[i]-x2[i])*(g1[i]-g2[i]);
        return s>=0;
}

double  Problem::getVolume()
{
    double s= 0.0;
    for(int i=0;i<lmargin.size();i++)
    {
        //if(fabs(rmargin[i]-lmargin[i])>s)
            s += (rmargin[i]-lmargin[i])*(rmargin[i]-lmargin[i]);
    }
    return sqrt(s);
}
QJsonObject    Problem::done(Data &x)
{
    return problem->done(x);
}

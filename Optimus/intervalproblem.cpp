#include "intervalproblem.h"
# include <QVariant>
# include <omp.h>

IntervalProblem::IntervalProblem(int d)
{

 threads=0;
 dimension=d;
 margin.resize((unsigned)d);
 for(unsigned int i=0;i<d;i++)
     margin[i]=Interval(-100,100);
}

void    IntervalProblem::setDimension(int d)
{
    dimension=d;
    margin.resize((unsigned)d);
    for(unsigned int i=0;i<d;i++)
        margin[i]=Interval(-100,100);
}

void    IntervalProblem::setParameter(QString name,QVariant value)
{
    //nothing here
}

void   IntervalProblem::setMargins(IntervalData &m)
{
    margin=m;
}

IntervalData IntervalProblem::getMargins() const
{
    return margin;
}

int    IntervalProblem::getDimension() const
{
    return dimension;
}

double  IntervalProblem::randomDouble()
{
    if(threads>0)
        return threadGen[omp_get_thread_num()].generateDouble();
    return randGen.generateDouble();
}

bool   IntervalProblem::isPointIn(Data &x) const
{
    for(int i=0;i<margin.size();i++)
    {
        if(x[i]<margin[i].leftValue() || x[i]>margin[i].rightValue())
            return false;
    }
    return true;
}

/*
Interval IntervalProblem::IntFunmin(IntervalData &x)
{
    return ifunmin(x);
}*/

double  IntervalProblem::getVolume(IntervalData &x) const
{
    double s=0;
    for(unsigned i=0;i<x.size();i++)
        s+=x[i].width();
    return s;
}

double  IntervalProblem::getVolume() const
{
    return 0.0;
}


void    IntervalProblem::printData(IntervalData &x)
{
    double totalWidth=0.0;
    for(int i=0;i<x.size();i++)
    {
        cout<<">>"<<x[i].toString().toStdString()<<",";
        totalWidth+=(x[i].rightValue()-x[i].leftValue());
    }
    cout<<" Width = "<<totalWidth<<  endl;
}

IntervalData    IntervalProblem::createRandomInterval()
{
    IntervalData x;
    x.resize(dimension);
    for(int i=0;i<dimension;i++)
        x[i]=margin[i];
    for(int i=0;i<dimension;i++)
        randomize(x,i);
    /*
    for(int i=0;i<dimension;i++)
    {
        x[i]=margin[i];
    }
    for(int i=0;i<dimension;i++)
    {
        int tries=5;
        for(int j=0;j<tries;j++)
        {
            int l=rand()%2;
            int r=rand()%2;
            double origLeft=x[i].left();
            if(l==1) origLeft/=2;
            if(origLeft<margin[i].left()) origLeft=margin[i].left();
            x[i]=interval(origLeft,x[i].right());
            double origRight=x[i].right();
            if(r==1)
            {origRight/=2;
                if(origRight<margin[i].left())
                    origRight=x[i].right();
                if(origRight<origLeft)
                {

                x[i]=interval(origRight,origLeft);
                }
                else x[i]=interval(origLeft,origRight);

            }
        }
    }*/
    return x;
}

void  IntervalProblem::setRandomSeedForThreads(int k,int t)
{
    threads=t;
    threadGen.resize(threads);
    for(int i=0;i<threads;i++)
        threadGen[i].seed(k+i);
}

void    IntervalProblem::randomize(IntervalData &x,int pos)
{
   double r1=randomDouble();
   double lvalue=x[pos].leftValue();
   double rvalue=x[pos].rightValue();
   x[pos]=lvalue+(rvalue-lvalue)*r1;
}

void  IntervalProblem::setRandomSeed(int k)
{
    randGen.seed(k);
}

bool   IntervalProblem::lowerValue(Interval &a,Interval &b)
{

     return a.leftValue()<b.leftValue()
       || (a.leftValue()<=b.leftValue() &&
           fabs(a.leftValue()-b.leftValue())<1e-6
           && a.width()<b.width());
}

bool    IntervalProblem::isPointIn(IntervalData &x) const
{
    for(int i=0;i<x.size();i++)
    {
        if(x[i].leftValue()<margin[i].leftValue()
         || x[i].rightValue()>margin[i].rightValue()) return false;
    }
    return true;
}
void    IntervalProblem::boundInterval(IntervalData &x)
{
    for(int i=0;i<x.size();i++)
    {
       double d1=(x[i].leftValue()<margin[i].leftValue())?margin[i].leftValue():x[i].leftValue();
       double d2=(x[i].rightValue()>margin[i].rightValue())?margin[i].rightValue():x[i].rightValue();
        d1=x[i].leftValue();
        d2=x[i].rightValue();
       if(d1>margin[i].rightValue()) d1=margin[i].rightValue();
        if(d1<margin[i].leftValue()) d1=margin[i].leftValue();
        if(d2<margin[i].leftValue()) d2=margin[i].leftValue();
        if(d2>margin[i].rightValue()) d2=margin[i].rightValue();

       if(d1>d2)
       {
           double t=d1;
           d1=d2;
           d2=t;
       }

       x[i]=Interval(d1,d2);
    }
}


void    IntervalProblem::init(QJsonObject x)
{
    //nothing here
}

double  IntervalProblem::funmin(Data &x)
{
    return 0.0;
}

void    IntervalProblem::granal(Data &x, Data &g)
{
    //nothing here
}

QJsonObject IntervalProblem::done(Data &x)
{
    return QJsonObject();
}

IntervalProblem::~IntervalProblem()
{
//nothing here
}

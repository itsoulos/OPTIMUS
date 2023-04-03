#include "dataset.h"
# include <stdio.h>
Dataset::Dataset(QString name)
{
    FILE *fp = fopen(name.toStdString().c_str(),"r");
    if(!fp) return;
    int d,c;
    fscanf(fp,"%d",&d);
    fscanf(fp,"%d",&c);
    xpoint.resize(c);
    ypoint.resize(c);
    for(int i=0;i<c;i++)
    {
        xpoint[i].resize(d);
        for(int j=0;j<d;j++)
        {
            fscanf(fp,"%lf",&xpoint[i][j]);
        }
        fscanf(fp,"%lf",&ypoint[i]);
    }
    fclose(fp);
}

void    Dataset::getXMargins(Data &xleft,Data &xright)
{
    int d = getdimension();
    xleft.resize(d);
    xright.resize(d);
    for(int i=0;i<xpoint.size();i++)
    {
        for(int j=0;j<d;j++)
        {
            if(i==0 || xpoint[i][j]<xleft[j])
                xleft[j]=xpoint[i][j];
            if(i==0 || xpoint[i][j]>xright[j])
                xright[j]=xpoint[i][j];
        }
    }
}

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

void Dataset::addPoint(Data &x,double y)
{
    xpoint.push_back(x);
    ypoint.push_back(y);
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

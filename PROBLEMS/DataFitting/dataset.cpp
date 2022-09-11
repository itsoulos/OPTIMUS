#include "dataset.h"
# include <math.h>
Dataset::Dataset()
{
dimension=0;
}

Dataset::Dataset(const char *filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return;
    int count,i,j;
    fscanf(fp,"%d",&dimension);
    fscanf(fp,"%d",&count);
    xpoint.resize(count);
    ypoint.resize(count);
    dclass.resize(0);
    for(i=0;i<count;i++)
    {
        xpoint[i].resize(dimension);
        for(j=0;j<dimension;j++)
        {
            fscanf(fp,"%lf",&xpoint[i][j]);
        }
        fscanf(fp,"%lf",&ypoint[i]);
        double dd = getNearestClass(ypoint[i]);
        if(dd<-1e+10) dclass.push_back(ypoint[i]);
    }
    fclose(fp);
}

double  Dataset::getNearestClass(double y)
{
    int found = -1;
    const double eps = 1e-5;
    for(int i=0;i<dclass.size();i++)
    {
        if(fabs(y-dclass[i])<eps)
        {
            found=i;
            break;
        }
    }
    if(found!=-1) return dclass[found];
    return -1e+100;
}

double Dataset::getMinDistClass(double y)
{
    int minPos=-1;
    double minDist=1e+100;
    for(int i=0;i<dclass.size();i++)
    {
        double df = fabs(y-dclass[i]);
        if(df<minDist)
        {
            minDist = df;
            minPos=i;
        }
    }
    return dclass[minPos];
}

double Dataset::gety(int pos)
{
    return ypoint[pos];
}


void    Dataset::normalizeZeroOne()
{
    Data max;
    Data min;
    int i,j;
    max.resize(dimension);
    min.resize(dimension);
    for(i=0;i<xpoint.size();i++)
    {
        for(j=0;j<dimension;j++)
        {
            if(i==0 || min[j]>xpoint[i][j])
                min[j]=xpoint[i][j];
            if(i==0 || max[j]<xpoint[i][j])
                max[j]=xpoint[i][j];
        }
    }
    for(i=0;i<xpoint.size();i++)
    {
        for(j=0;j<dimension;j++)
        {
            xpoint[i][j]=(xpoint[i][j]-min[j])/(max[j]-min[j]);
        }
    }
}


int     Dataset::getdimension() const
{
    return dimension;
}

int     Dataset::getpatterns() const
{
    return xpoint.size();
}

Data    Dataset::getpoint(int pos)
{
    return xpoint[pos];
}

Dataset::~Dataset()
{

}

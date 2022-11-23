#include "dataset.h"
# include <QLocale>
# include <QFile>
# include <QTextStream>
using namespace RbfTestName;

Dataset::Dataset()
{
dimension=0;
}

Dataset::Dataset(const char *filename)
{
    QLocale curLocale(QLocale("en_US"));
    QLocale::setDefault(curLocale);
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream st(&fp);

    int count,i,j;
    st>>dimension;
    st>>count;

    xpoint.resize(count);
    ypoint.resize(count);
    for(i=0;i<count;i++)
    {
        xpoint[i].resize(dimension);
        for(j=0;j<dimension;j++)
        {
            st>>xpoint[i][j];
        }
        st>>ypoint[i];
    }
    fp.close();
}

double Dataset::gety(int pos)
{
    return ypoint[pos];
}


void    Dataset::normalizeZeroOne()
{
    QVector<double> max;
    QVector<double> min;
    int i,j;
    max.resize(dimension);
    max.fill(-1e+100);
    min.resize(dimension);
    min.fill(1e+100);
    for(i=0;i<xpoint.size();i++)
    {
        for(j=0;j<dimension;j++)
        {
            if(min[j]>xpoint[i][j])
                min[j]=xpoint[i][j];
            if(max[j]<xpoint[i][j])
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

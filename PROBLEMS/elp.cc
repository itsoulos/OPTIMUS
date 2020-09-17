# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
using namespace std;


extern "C"
{

int dimension=8;

void    init(QJsonObject data)
{
    if(data.contains("dimension"))
        dimension=data["dimension"].toInt();
}

int	getdimension()
{
    return dimension;
}

void 	getmargins(vector<Interval> &x)
{
	for(int i=0;i<x.size();i++)
        x[i]=Interval(-dimension*1.0,dimension*1.0);
}


double	funmin(vector<double> &x)
{
    double sum1=0.0;
          for(int i=0;i<dimension;i++)
                  sum1+=(i+1)*pow(x[i],3.0);
          return sum1;

}

void    granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<dimension;i++)
        g[i]=3.0*(x[i]-i-1)*(x[i]-i-1)*(i+1.0);

}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

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
    if(data.contains("natoms"))
        dimension=data["natoms"].toString().toInt();
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

static double dmax(double a,double b)
{
	return a>b?a:b;
}

void	granal(vector<double>&x,vector<double>&g)
{
	for(int i=0;i<dimension;i++)
	{
		double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
		x[i]+=eps;
		double v1=funmin(x);
		x[i]-=2.0 *eps;
		double v2=funmin(x);
		g[i]=(v1-v2)/(2.0 * eps);
		x[i]+=eps;
	}
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

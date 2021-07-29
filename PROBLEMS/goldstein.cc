# include <math.h>
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

void    init(QJsonObject data)
{

}

	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}
int	getdimension()
{
	return 2;
}

void    getmargins(vector<Interval> &x)
{
        for(int i=0;i<x.size();i++)
                x[i]=Interval(-2,2);
}

double	funmin(vector<double> &x)
{
	double x1=x[0];
	double x2=x[1];
	return (1.0+pow(x1+x2+1.0,2.0)*(19-14*x1+3*x1*x1-14*x2+6*x1*x2+3*x2*x2))*
		(30+pow(2*x1-3*x2,2.0)*(18-32*x1+12*x1*x1+48*x2-36*x1*x2+27*x2*x2));
}

void    granal(vector<double> &x,vector<double> &g)
{
	 for(int i=0;i<getdimension();i++)
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

}

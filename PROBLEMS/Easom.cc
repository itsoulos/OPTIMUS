# include <math.h>
# include "interval.h"
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


	static double dmax(double x,double y)
	{
		return x>y?x:y;
	}
int	getdimension()
{
	return 2;
}

void	getleftmargin(double *x)
{
	for(int i=0;i<getdimension();i++) x[i]=-100.0;
}

void	getrightmargin(double *x)
{
	for(int i=0;i<getdimension();i++) x[i]= 100.0;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-100.0,100.0);
    }
}
double	funmin(vector<double> &x)
{
	return -cos(x[0])*cos(x[1]*exp(-(pow(x[0]-M_PI,2.0)+pow(x[1]-M_PI,2.0))));
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
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

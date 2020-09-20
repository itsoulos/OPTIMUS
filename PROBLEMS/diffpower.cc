# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
using namespace std;
extern "C"
{

	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}
	int DIMENSION=100;
void    init(QJsonObject data)
{
    if(data.contains("natoms"))
        DIMENSION=data["natoms"].toString().toInt();
    if(data.contains("dimension")) 
	    DIMENSION=data["dimension"].toString().toInt();
}
void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-1e+10,1e+10);
		    }
		    }

int	getdimension()
{
	return DIMENSION;
}


double	funmin(vector<double> &x)
{
	double sum=0.0;
	for(int i=0;i<DIMENSION;i++)
		sum+=pow(fabs(x[i]),2.0+10.0*(i*1.0)/(DIMENSION-1.0));
	return sum;
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

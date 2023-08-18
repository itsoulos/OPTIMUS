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
# include <stdio.h>

extern "C"
{


	static double dmax(double x,double y)
	{
		return x>y?x:y;
	}
int dimension=4;
int N  = dimension;
int	getdimension()
{
    return dimension;
}

void    init(QJsonObject data)
{
    if(data.contains("natoms"))
    {
        dimension=data["natoms"].toString().toInt();
	N=dimension;
    }
}
void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-30,30);
    }
}

double	funmin(vector<double> &x)
{
	double sum=0.0;
	for(int i=0;i<N-1;i++)
	{
		sum=sum+100.0*pow(x[i]*x[i]-x[i+1],2.0)+pow(x[i],2.0);
	}
	return sum;
}

void    granal(vector<double> &x,vector<double> &g)
{
	 for(int i=0;i<getdimension();i++)
         {
		 g[i]=0.0;
	 }
	 for(int i=0;i<getdimension()-1;i++)
	 {
		 g[i]+=100.0*4.0*x[i]*(x[i]*x[i]-x[i+1])+2.0*x[i];
		 g[i+1]+=-100.0*2.0*(x[i]*x[i]-x[i+1]);
	 }
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

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

# define DIMENSION	3
int dimension=3;
int	getdimension()
{
    return dimension;
}

void    init(QJsonObject data)
{
    if(data.contains("natoms"))
        dimension=data["natoms"].toString().toInt();
}
void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-10.0,10.0);
    }
}
double	funmin(vector<double> &x)
{
	double sum=0.0;
	for(int i=1;i<DIMENSION-1;i++)
		sum+=pow(x[i]-1.0,2.0)*(1.0+pow(sin(3*M_PI*x[i+1]),2.0));
	return 0.1 *(pow(sin(3*M_PI*x[0]),2.0))*sum+pow(x[DIMENSION-1]-1.0,2.0)*(1.0+pow(sin(2.0*M_PI*x[DIMENSION-1]),2.0));
}

void    granal(vector<double> &x,vector<double> &g)
{
	for(int i=0;i<DIMENSION;i++) g[i]=0.0;
	double sum=0.0;
	for(int i=1;i<DIMENSION-1;i++)
		sum+=pow(x[i]-1.0,2.0)*(1.0+pow(sin(3*M_PI*x[i+1]),2.0));
	g[0]=0.1 * sum * 2 * sin(3.0 * M_PI *x[0])* 3 * M_PI *cos(3.0 * M_PI *x[0]);
	for(int i=1;i<DIMENSION-1;i++)
	{
		double m=0.1 * pow(sin(3*M_PI*x[0]),2.0);
		g[i]+=2.0*(x[i]-1.0)*(1.0+pow(sin(3*M_PI*x[i+1]),2.0))*m;
		g[i+1]+=pow(x[i]-1.0,2.0)*2.0*3*M_PI*sin(3*M_PI*x[i+1])*cos(3*M_PI*x[i+1])*m;
	}
	g[DIMENSION-1]+=2.0*(x[DIMENSION-1]-1.0)*(1.0+pow(sin(2.0*M_PI*x[DIMENSION-1]),2.0))+
		pow(x[DIMENSION-1]-1.0,2.0)*2.0*2.0*M_PI*sin(2.0*M_PI*x[DIMENSION-1])*
		cos(2.0*M_PI*x[DIMENSION-1]);
}

}

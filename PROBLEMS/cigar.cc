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
# define DIMENSION 10
int	getdimension()
{
	return DIMENSION;
}

void	getleftmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]=-1.0e+10;
}

void	getrightmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]= 1.0e+10;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-1e+10,1e+10);
    }
}
double	funmin(vector<double> &x)
{
	double sum=x[0]*x[0];
	for(int i=1;i<DIMENSION;i++)
		sum+=pow(10.0,6.0) * x[i]*x[i];
	return sum;
}

void    granal(vector<double> &x,vector<double> &g)
{
	g[0]=2.0 * x[0];
	for(int i=1;i<DIMENSION;i++)
		g[i]=pow(10.0,6.0)* 2.0 * x[i];
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

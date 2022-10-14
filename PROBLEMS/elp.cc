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
# define	DIMENSION 10
int	getdimension()
{
	return DIMENSION;
}

void	getleftmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]=-DIMENSION;
}

void	getrightmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]= DIMENSION;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(0.0,10.0);
    }
}
double	funmin(vector<double> &x)
{
	double sum1=0.0;
	for(int i=0;i<DIMENSION;i++)
		sum1+=pow(pow(10.0,6.0),i*1.0/(getdimension()-1.0))*x[i]*x[i];
	return sum1;
}

void    granal(vector<double> &x,vector<double> &g)
{
	for(int i=0;i<DIMENSION;i++)
		g[i]=pow(pow(10.0,6.0),i*1.0/(getdimension()-1.0))*2.0*x[i];
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

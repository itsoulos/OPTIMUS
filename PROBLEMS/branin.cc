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
	
	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}

    void    init(QJsonObject data)
    {

    }

int	getdimension()
{
	return 2;
}

void 	getmargins(vector<Interval> &x)
{
    x[0]=Interval(-5.0,10.0);
    x[1]=Interval(0.0,15.0);
}

double	funmin(vector<double>& x)
{
	double x1=x[0];
	double x2=x[1];
	return pow(x2-5.1/(4*M_PI*M_PI)*x1*x1+5.0/M_PI-6.0,2.0)+10.0*(1.0-1.0/(8.0*M_PI))*cos(x1)+10.0;
}

void    granal(vector<double> &x,vector<double> &g)
{
	double x1=x[0];
	double x2=x[1];
	g[0]=-2.0*x1*5.1/(4*M_PI*M_PI)*2.0*(x2-5.1/(4*M_PI*M_PI)*x1*x1+5.0/M_PI-6.0)
		-10.0*(1.0-1.0/(8.0*M_PI))*sin(x1);
	g[1]=2.0*(x2-5.1/(4*M_PI*M_PI)*x1*x1+5.0/M_PI-6.0);
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

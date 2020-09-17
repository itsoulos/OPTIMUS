# include <math.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include "interval.h"
using namespace std;


extern "C"
{

void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 2;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
        x[i]=Interval(-100*1.0,100*1.0);
}

double	funmin(vector<double> &x)
{
	return x[0]*x[0]+2.0*x[1]*x[1]-0.3*cos(3.0*M_PI*x[0])-0.4*cos(4.0*M_PI*x[1])+0.7;
}

void    granal(vector<double> &x,vector<double> &g)
{
	g[0]=2.0*x[0]+0.9*M_PI*sin(3.0*M_PI*x[0]);
	g[1]=4.0*x[1]+1.6*M_PI*sin(4.0*M_PI*x[1]);
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

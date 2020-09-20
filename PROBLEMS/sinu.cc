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

int  DIMENSION=8;

void    init(QJsonObject data)
{
    if(data.contains("natoms"))
        DIMENSION=data["natoms"].toString().toInt();
}

int	getdimension()
{
	return DIMENSION;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(0.0,M_PI);
    }
}

double	funmin(vector<double> &x)
{
	double p1=1.0,p2=1.0;
	const double z=M_PI/6;
	for(int i=0;i<DIMENSION;i++)
	{
		p1*=sin(x[i]-z);
		p2*=sin(5*(x[i]-z));
	}
	return -2.5 * p1 -p2;
}

void    granal(vector<double> &x,vector<double> &g)
{
	double p1=1.0,p2=1.0;
	const double z=M_PI/6;
	for(int i=0;i<DIMENSION;i++)
	{
		p1=1.0,p2=1.0;
		for(int j=0;j<DIMENSION;j++)
		{
			if(i==j) continue;
			p1*=sin(x[j]-z);
			p2*=sin(5*(x[j]-z));
		}
		g[i]=-2.5 * cos(x[i]-z)*p1-5.0*p2*cos(5*(x[i]-z));
	}
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

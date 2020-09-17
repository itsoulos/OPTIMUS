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
int N=6;

void    init(QJsonObject data)
{
    if(data.contains("dimension"))
        N=data["dimension"].toInt();
}

int	getdimension()
{
	return N;
}
void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-5.0,5.0);
    }
}

double	funmin(vector<double> &x)
{
	double sum=0.0;
	for(int i=0;i<N;i++)
		sum+=0.5 * (pow(x[i],4.0)-16.0 * pow(x[i],2.0)+5.0*x[i]);
	return sum;
}

void    granal(vector<double> &x,vector<double> &g)
{
	for(int i=0;i<N;i++)
	{
		g[i]=0.5 * (4.0*pow(x[i],3.0)-32*x[i]+5.0);
	}
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

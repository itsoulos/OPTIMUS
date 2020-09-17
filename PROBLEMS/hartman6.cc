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

	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}

int	getdimension()
{
	return 6;
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-0.0,1.0);
    }
}

double	funmin(vector<double> &x)
{
	static double a[4][6]={{10,3,17,3.5,1.7,8},
		{0.05,10,17,0.1,8,14},
		{3,3.5,1.7,10,17,8},
		{17,8,0.05,10,0.10,14}};
	static double c[4]={1,1.2,3,3.2};
	static double p[4][6]={{0.1312,0.1696,0.5569,0.0124,0.8283,0.5886},
		{0.2329,0.4135,0.8307,0.3736,0.1004,0.9991},
		{0.2348,0.1451,0.3522,0.2883,0.3047,0.6650},
		{0.4047,0.8828,0.8732,0.5743,0.1091,0.0381}};
	double sum=0.0;
	for(int i=0;i<4;i++)
	{
		double s=0.0;
		for(int j=0;j<6;j++) s=s+a[i][j]*pow(x[j]-p[i][j],2.0);
		sum=sum+c[i]*exp(-s);
	}
	return -sum;
}

void    granal(vector<double> &x,vector<double> &g)
{
	static double a[4][6]={{10,3,17,3.5,1.7,8},
		{0.05,10,17,0.1,8,14},
		{3,3.5,1.7,10,17,8},
		{17,8,0.05,10,0.10,14}};
	static double c[4]={1,1.2,3,3.2};
	static double p[4][6]={{0.1312,0.1696,0.5569,0.0124,0.8283,0.5886},
		{0.2329,0.4135,0.8307,0.3736,0.1004,0.9991},
		{0.2348,0.1451,0.3522,0.2883,0.3047,0.6650},
		{0.4047,0.8828,0.8732,0.5743,0.1091,0.0381}};
	for(int i=0;i<getdimension();i++) g[i]=0.0;
	for(int j=0;j<getdimension();j++) 
	{
		double pj=0.0;
		for(int i=0;i<4;i++)
		{
			double sum1=0.0;
			for(int k=0;k<getdimension();k++)
				sum1+=a[i][k]*pow(x[k]-p[i][k],2.0);
			pj+=c[i]*exp(-sum1)*(-2.0)*a[i][j]*(x[j]-p[i][j]);
		}
		g[j]=-pj;
	}
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

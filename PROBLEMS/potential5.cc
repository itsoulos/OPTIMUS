# include <math.h>
# include <gaol/gaol.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
using namespace std;

extern "C"{

void    init(QJsonObject data)
{

}
void 	getmargins(vector<interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=interval(0.0,1.0);
    }
}
# define 	NAT	5
static double dmax(double a,double b){return a>b?a:b;}
int	getdimension()
{
	return	3 * NAT;
}



double	Pow(double x,int n)
{
	double p=1.0;
	for(int i=0;i<n;i++) p*=x;
	return p;
}



interval	ipot(interval r)
{
    interval	eps=interval(1.0,1.0);
    interval 	sig =interval(1.0,1.0);
    interval	eps4=4.0*eps;
    interval sbyr6=pow(sig/r,6);
    return eps4 * sbyr6*(sbyr6-1.0);
}

interval ifunmin(vector<interval> &x)
{
    interval value=interval(0.0,0.0);
    vector<interval> xx;xx.resize(NAT);
    vector<interval> yy;yy.resize(NAT);
    vector<interval> zz;zz.resize(NAT);
    for(int i=1;i<=NAT;i++)
    {
        int i3=3 * i;
        xx[i-1]=x[i3-2-1];
        yy[i-1]=x[i3-1-1];
        zz[i-1]=x[i3-1];
    }
    for(int i=1;i<=NAT;i++)
    {
        for(int j=1;j<=NAT;j++)
        {
            if(i==j) continue;
            interval dx=xx[i-1]-xx[j-1];
            interval dy=yy[i-1]-yy[j-1];
            interval dz=zz[i-1]-zz[j-1];
            interval rij=sqrt(pow(dx,2.0)+pow(dy,2.0)+pow(dz,2.0));
            interval d=ipot(rij);
          if(isinff(d.left()) || isinf(d.right())) return 1e+100;
        //  cout<<"IPOT "<<d<<endl;

          value=value+d;
        }
    }

    return 0.5 * value;
}

double	pot(double r)
{
	double	eps=1.0;
	double 	sig =1.0;
	double	eps4=4.0 *eps;
	double sbyr6=Pow(sig/r,6);
	return eps4 * sbyr6*(sbyr6-1.0);
}

double	funmin(vector<double>  &x)
{
	double value=0.0;
    vector<double> xx;xx.resize(NAT);
    vector<double> yy;yy.resize(NAT);
    vector<double> zz;zz.resize(NAT);
	for(int i=1;i<=NAT;i++)
	{
		int i3=3 * i;
		xx[i-1]=x[i3-2-1];
		yy[i-1]=x[i3-1-1];
		zz[i-1]=x[i3-1];
	}
	for(int i=1;i<=NAT;i++)
	{
		for(int j=1;j<=NAT;j++)
		{
			if(i==j) continue;
			double dx=xx[i-1]-xx[j-1];
			double dy=yy[i-1]-yy[j-1];
			double dz=zz[i-1]-zz[j-1];
			double rij=sqrt(dx*dx+dy*dy+dz*dz);
            double dpot=pot(rij);
            printf("DPOT = %lf \n",dpot);
            value=value+dpot;
		}
	}

	return 0.5 * value;
}


void	granal(vector<double>&x,vector<double>&g)
{
	for(int i=0;i<3 * NAT;i++)
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
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

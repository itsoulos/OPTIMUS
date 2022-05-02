# include <math.h>
# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
using namespace std;

#pragma gcc optimize("Ofast")
/*	This is a sample file for ODE,
 *	written in C++. The meaning of the functions is 
 *	as follows:
 *		1. getx0():  Return the left boundary of the equation.
 *		2. getx1():  Return the right boundary of the equation.
 *		3. getkind():Return the kind of the equation.
 *		4. getnpoints(): Return the number points in which the system
 *				will try to solve the ODE.
 *		5. getf0():  Return the left boundary condition.
 *		6. getf1():  Return the right boundary condition.
 *		7. getff0(): Return the derivative of the left boundary condition.
 *		8. ode1ff(): Return the equation for first order equations.
 *		9. ode2ff(): Return the equation for second order equations.
 * */
extern "C"
{

double	getx0()
{
	return 1;
}

double	getx1()
{
	return 2;
}

int	getkind()
{
	return 1;
}

int	getnpoints()
{
	return 100;
}

double	getf0()
{
	return 3;
}

double	getff0()
{
	return 10.0;
}

double	getf1()
{
	return sin(10.0);
}

double	ode1ff(double x,double y,double yy)
{
	return yy -(2*x-y)/x;
}

double	dode1ff(double x,double y,double yy,double dy,double dyy)
{
	return dyy+dy/x;
}

double	ode2ff(double x,double y,double yy,double yyy)
{
}

double	dode2ff(double x,double y,double yy,double yyy,double dy,double dyy,double dyyy)
{
}

void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 5;
}

void    getmargins(vector<Interval> &x)
{
        for(int i=0;i<x.size();i++)
                x[i]=Interval(-100,100);
}


//f(x)
double	funmin(vector<double> &a)
{
	double x=0.0;
	double sum = 0.0;
	double p0=0.0;
	int first = 1;
	for(x=getx0();x<=getx1();x+=(getx1()-getx0())/getnpoints())
	{
		//compute p(x), dpx
		double px = 0.0;
		double dpx = 0.0;
		for(int i=1;i<=getdimension();i++)
		{
			px = px + pow(x,i)*a[i-1];
			dpx  = dpx + i*pow(x,i-1)*a[i-1];
		}
		if(first) {p0=px;first=0;}
		sum = sum + pow(ode1ff(x,px,dpx),2.0);
	}
	return sum+100.0 * pow(p0-getf0(),2.0);
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

static double dmax(double a,double b){return a>b?a:b;}
void	granal(vector<double> &x,vector<double> &g)
{
	for(int i=0;i<getdimension();i++)
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

}

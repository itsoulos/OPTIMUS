# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <vector>
# define MARGIN_EPS 1e-5
# define LAMBDA     1e+5
using namespace std;

/*	g02
 *	From: Differential evolution with dynamic stochastic selection
 *	             for constrained optimization
              Min Zhang, Wenjian Luo, Xufa Wang, Information Sciences
	      Best located value: 0.803619
 *
 * */
extern "C"
{

/*	Return the dimension of the objective function.
 * */
int	getdimension()
{
	return 10;
}

/*
 * Return the number of equalities of the objective problem.
 */
int	geteq()
{
	return 0;
}

/*	Return the number of inequalities of the objective problem.
 * */
int	getineq()
{
	return 2;
}

/*	Return the left bounds of the objective function.
 * */
void	getleftmargin(double *x)
{
	for(int i=0;i<getdimension();i++) x[i]=0.0;
}

/*	Return the right bounds of the objective function.
 * */
void	getrightmargin(double *x)
{
	for(int i=0;i<getdimension();i++) x[i]=10.0;
}

void    getmargins(vector<Interval> &x)
{
	int n=getdimension();
	double xl[n];
	double xr[n];
	getleftmargin(xl);
	getrightmargin(xr);
        for(int i=0;i<x.size();i++)
                x[i]=Interval(xl[i],xr[i]);
}

/*	Store in the array eq the values 0 and 1 for the equalities of the problem.
 * */
void	feq(vector<double> &x,vector<double> &eq)
{
}

/*	Store in the array eq the values 0 and 1 for the inequalities of the problem.
 * */
void	fineq(vector<double> &x,vector<double> &ineq)
{
	double sum=0.0,prod=1.0;
	for(int i=0;i<getdimension();i++)
	{
		sum+=x[i];
		prod*=x[i];
	}
	ineq[0]=0.75-prod;
	ineq[1]=sum-7.5*getdimension();
}

static double dmax(double a,double b)
{
	return a>b?a:b;
}
/*	Return the objective function evaluated at a feasible point.
 * */
double	funmin(vector<double> &x)
{
	double sum1=0.0,prod=1.0,sum2=0.0;
	for(int i=0;i<getdimension();i++) 
	{
		double v=cos(x[i]);
		sum1+= v * v * v * v;
	 	prod*= v * v;
		sum2+=(i+1)*x[i]*x[i];
	}
	double v1= -fabs((sum1-2*prod)/sqrt(sum2));
	vector<double> temp_eq,temp_ineq;
	temp_eq.resize(geteq());
	temp_ineq.resize(getineq());
	feq(x,temp_eq);
        fineq(x,temp_ineq);
        double v2=0.0,v3=0.0;

        for(int i=0;i<temp_eq.size();i++)
        {
                v2=v2+pow(temp_eq[i],2.0);
        }

        for(int i=0;i<temp_ineq.size();i++)
        {
                if(temp_ineq[i]>0) v3=v3+pow(temp_ineq[i],2.0);
        }
        double l=dmax(LAMBDA,1000 * fabs(v1));
        return (v1+l*(v2+v3))+0.0;//100*(1.0-fes);


}

/*	Return the gradient of the objective function evaluated at a feasible point.
 * */
void    granal(vector<double> &x,vector<double>&g)
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

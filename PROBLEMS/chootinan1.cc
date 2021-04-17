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

extern "C"
{

void    init(QJsonObject data)
{

}
/*	Return the dimension of the objective function.
 * */
int	getdimension()
{
	return 13;
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
	return 9;
}

/*	Return the left bounds of the objective function.
 * */
void	getleftmargin(double *x)
{
	for(int i=0;i<13;i++) x[i]=0.0;
}

/*	Return the right bounds of the objective function.
 * */
void	getrightmargin(double *x)
{
	for(int i=0;i<13;i++) x[i]=1.0;
	x[9]=x[10]=x[11]=100.0;
}

static double dmax(double a,double b)
{
	return a>b?a:b;
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
 double x1=x[0],x2=x[1],x3=x[2],x4=x[3],x5=x[4],x6=x[5],x7=x[6],x8=x[7],x9=x[8],x10=x[9],x11=x[10],x12=x[11],x13=x[12];
 ineq[0]=-(10-(2*x1+2*x2+x10+x11));
 ineq[1]=-(10-(2*x1+2*x3+x10+x12));
 ineq[2]=-(10-(2*x2+2*x3+x11+x12));
 ineq[3]=-(8*x1-x10);
 ineq[4]=-(8*x2-x11);
 ineq[5]=-(8*x3-x12);
 ineq[6]=-(2*x4+x5-x10);
 ineq[7]=-(2*x6+x7-x11);
 ineq[8]=-(2*x8+x9-x12);
}

/*	Return the objective function evaluated at a feasible point.
 * */
double	funmin(vector<double> &x)
{
	double sum1=0.0,sum2=0.0,sum3=0.0;
	for(int i=0;i<4;i++) sum1=sum1+x[i];
	for(int i=0;i<4;i++) sum2=sum2+x[i]*x[i];
	for(int i=4;i<13;i++) sum3=sum3+x[i];
	double v1=5*sum1-5*sum2-sum3;
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
void    granal(vector<double> &x,vector<double> &g)
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

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
	return 4;
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
	return 7;
}

/*	Return the left bounds of the objective function.
 * */
void	getleftmargin(double *x)
{
	x[0]=x[3]=0.1;
	x[1]=x[2]=0.1;
}

/*	Return the right bounds of the objective function.
 * */
void	getrightmargin(double *x)
{
	x[0]=x[3]=2;
	x[1]=x[2]=10;
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
	double x1=x[0],x2=x[1],x3=x[2],x4=x[3];
	double G=12*1e+6;
	double E=30.0*1e+6;
	double L=14.0;
	double P=6000.0;
	double PC=4.013*E*sqrt(x3*x3*pow(x4,6.0)/36.0)/(L*L)*(1.0-x3/(2*L)*sqrt(E/(4.0*G)));
	double D=4.0*P*L*L*L/(E*x3*x3*x3*x4);
	double S=6.0*P*L/(x4*x3*x3);
	double J=2*(sqrt(2.0)*x1*x2*(x2*x2/12.0+pow(x1/2.0+x3/2.0,2.0)));
	double R=sqrt(x2*x2/4.0+pow(x1/2.0+x3/2.0,2.0));
	double Q=P*(L+x2/2.0);
	double T1=P/sqrt(sqrt(2.0)*x1*x2);
	double T2=Q*R/J;
	double T=sqrt(T1*T1+2*T1*T2*x2/(2.0*R)+T2*T2);
	ineq[0]=T-13600;
	ineq[1]=S-30000;
	ineq[2]=x1-x4;
	ineq[3]=0.10471*x1*x1+0.04811*x3*x4*(14.0+x2)-5.0;
	ineq[4]=0.125-x1;
	ineq[5]=D-0.25;
	ineq[6]=P-PC;
}

/*	Return the gradient of the objective function evaluated at a feasible point.
 * */
void    granal(vector<double> &x,vector<double> &g)
{
	g[0]=2.0*x[0]*1.10471*x[1];
	g[1]=1.0471*x[0]*x[0]+0.04811*x[2]*x[3];
	g[2]=0.04811*x[3]*(14.0+x[1]);
	g[3]=0.04811*x[2]*(14.0+x[1]);
}

static double dmax(double a,double b)
{
	return a>b?a:b;
}
/*	Return the objective function evaluated at a feasible point.
 * */
double	funmin(vector<double> &x)
{
	vector<double> temp_eq,temp_ineq;
	temp_eq.resize(geteq());
	temp_ineq.resize(getineq());
	feq(x,temp_eq);
        fineq(x,temp_ineq);
        double v1=0.0,v2=0.0,v3=0.0;

        for(int i=0;i<temp_eq.size();i++)
        {
                v2=v2+pow(temp_eq[i],2.0);
        }

        for(int i=0;i<temp_ineq.size();i++)
        {
                if(temp_ineq[i]>0) v3=v3+pow(temp_ineq[i],2.0);
        }
	v1= 1.10471*x[0]*x[0]*x[1]+0.04811*x[2]*x[3]*(14.0+x[1]);
        double l=dmax(LAMBDA,1000 * fabs(v1));


        return (v1+l*(v2+v3))+0.0;//100*(1.0-fes);
}
}

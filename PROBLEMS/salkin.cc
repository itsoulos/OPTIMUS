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
	return 5;
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
	return 4;
}

/*	Return the left bounds of the objective function.
 * */
void	getleftmargin(double *x)
{
	x[0]=1;x[1]=80;x[2]=30;x[3]=145;x[4]=0;
}

/*	Return the right bounds of the objective function.
 * */
void	getrightmargin(double *x)
{
	x[0]=4;x[1]=88;x[2]=35;x[3]=150;x[4]=2;
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

static double dmax(double a,double b)
{
	return a>b?a:b;
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
	ineq[0]=25*x[0]-40*x[1]+16*x[2]+21*x[3]+x[4]-300.0;
	ineq[1]=x[0]+20*x[1]-50*x[2]+x[3]-x[4]-200.0;
	ineq[2]=60*x[0]+x[1]-x[2]+2*x[3]+x[4]-600.0;
	ineq[3]=-7*x[0]+4*x[1]+15*x[2]-x[3]+65*x[4]-700.0;
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
	v1= -(3*x[0]+x[1]+2*x[2]+x[3]-x[4]);
        double l=dmax(LAMBDA,1000 * fabs(v1));


        return (v1+l*(v2+v3))+0.0;//100*(1.0-fes);
}

//f'(x)
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

QJsonObject    done(vector<double> &x)
{
	
	funmin(x);
	//int fes=isFeasible(x);
	//printf("Problem is Feasible?%s\n",(fes==1)?"YES":"NO");
    return QJsonObject();
}


}

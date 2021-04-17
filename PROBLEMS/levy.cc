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
	return 2;
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
	return 1;
}

/*	Return the left bounds of the objective function.
 * */
void	getleftmargin(double *x)
{
	x[0]=x[1]=0;
}

/*	Return the right bounds of the objective function.
 * */
void	getrightmargin(double *x)
{
	x[0]=x[1]=1;
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
	double a=2.0;
	double b=0.25;
	ineq[0]=-(((x[0]-1)*(x[0]-1)+(x[1]-1))*(1/(2.0*a*a)-1.0/(2*b*b))+(x[0]-1)*(x[1]-1)*(1.0/(a*a)-1.0/(b*b))-1.0);
}

int     isFeasible(vector<double> &x)
{
	int neq=geteq();
	int nineq=getineq();
	double lmargin[getdimension()];
	double rmargin[getdimension()];
	getleftmargin(lmargin);
	getrightmargin(rmargin);
        for(int i=0;i<x.size();i++)
        {
                //if(x[i]-rmargin[i]>MARGIN_EPS || lmargin[i]-x[i]>MARGIN_EPS) return 0;
                if(x[i]>rmargin[i] || lmargin[i]>x[i]) return 0;
        }
        if(neq)
        {
                vector<double> k;
                k.resize(neq);
                feq(x,k);
                double eq_max=-1;
                double sum=0.0;
                for(int i=0;i<k.size();i++)
                {
                        if(fabs(k[i])>eq_max) eq_max=fabs(k[i]);
                        sum+=k[i]*k[i];
                }
                if(sum>1e-5) return 0;
                //if(eq_max>MARGIN_EPS*10) return 0;
        }
        vector<double> t;
        t.resize(nineq);
        fineq(x,t);
        if(t.size()) for(int i=0;i<t.size();i++) if(t[i]>MARGIN_EPS) {return 0;}
        return 1;
}
static double dmax(double a,double b)
{
	return a>b?a:b;
}
/*	Return the objective function evaluated at a feasible point.
 * */
double	funmin(vector<double> &x)
{
	int fes=isFeasible(x);

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
	v1=-x[0]-x[1];
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
	int fes=isFeasible(x);
	printf("Problem is Feasible?%s\n",(fes==1)?"YES":"NO");
    return QJsonObject();
}


}

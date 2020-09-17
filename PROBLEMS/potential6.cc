# include <math.h>
# include <stdio.h>
extern "C"{
# define 	NAT	6
static double dmax(double a,double b){return a>b?a:b;}
int	getdimension()
{
	return	3 * NAT;
}

int	hasmargins()
{
	return 1;
}

void	getleftmargin(double *x)
{
	for(int i=0;i<3*NAT;i++) x[i]= -1;
}

void	getrightmargin(double *x)
{
	for(int i=0;i<3*NAT; i++) x[i]= 1;
}


double	Pow(double x,int n)
{
	double p=1.0;
	for(int i=0;i<n;i++) p*=x;
	return p;
}

double	pot(double r)
{
	double	eps=1.0;
	double 	sig =1.0;
	double	eps4=4.0 *eps;
	double sbyr6=Pow(sig/r,6);
	return 4.0 *(pow(1.0/r,12.0)-pow(1.0/r,6.0));
	return eps4 * sbyr6*(sbyr6-1.0);
}

double	funmin(double *x)
{
	double value=0.0;
	double  *xx=new double[NAT];
	double *yy =new double[NAT];
	double *zz =new double[NAT];
	for(int i=1;i<=NAT;i++)
	{
		int i3=3 * i;
		xx[i-1]=x[i3-2-1];
		yy[i-1]=x[i3-1-1];
		zz[i-1]=x[i3-1];
	}
	for(int i=1;i<=NAT-1;i++)
	{
		for(int j=i+1;j<=NAT;j++)
		{
			if(i==j) continue;
			double dx=xx[i-1]-xx[j-1];
			double dy=yy[i-1]-yy[j-1];
			double dz=zz[i-1]-zz[j-1];
			double rij=sqrt(dx*dx+dy*dy+dz*dz);
			value=value+pot(rij);
		}
	}
	delete[] xx;
	delete[] yy;
	delete[] zz;
	return  value;
}


void	granal(double *x,double *g)
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

}

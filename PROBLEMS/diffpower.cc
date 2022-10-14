# include <math.h>

extern "C"
{

	static double dmax(double a,double b)
	{
		return a>b?a:b;
	}
# define DIMENSION 5
int	getdimension()
{
	return DIMENSION;
}

void	getleftmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]=-1.0;
}

void	getrightmargin(double *x)
{
	for(int i=0;i<DIMENSION;i++)
		x[i]= 1.0;
}

double	funmin(double *x)
{
	double sum=0.0;
	for(int i=0;i<DIMENSION;i++)
		sum+=pow(fabs(x[i]),(i+2.0));
	return sum;
}

void    granal(double *x,double *g)
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

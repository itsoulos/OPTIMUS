# include <math.h>
/*	This is a sample file for systems of ODE's,
 *	written in C++. The meaning of the functions is 
 *	as follows:
 *		1. getx0():  Return the left boundary of the equation.
 *		2. getx1():  Return the right boundary of the equation.
 *		3. getnode():Return the amount of the equations.
 *		4. getnpoints(): Return the number points in which the system
 *				will try to solve the ODE.
 *		5. systemfun(node,x,y,yy): Return the system of equations to 
 *				be solved. In parameter y we put the value of y's
 *				and in the table yy the value of the derivatives of y's.
 *		6. systemf0(node, f0): It returns in table f0 the left boundaries of 
 *				the equations.
 * */
extern "C"
{

double	getx0()
{
	return 0.0;
}

double	getx1()
{
	return 1.0;
}

int	getnode()
{
	return 2;
}

int	getnpoints()
{
	return 10;
}

void	systemfun(int node, double x, double *y, double *yy,double *res)
{
	res[0]=yy[0]-(cos(x)+y[0]*y[0]+y[1]-x*x-sin(x)*sin(x));
	res[1]=yy[1]-(2.0*x-x*x*sin(x)+y[0]*y[1]);
}

void	systemder(int node, double x, double *y, double *yy,double dy,double dyy,double *res)
{
	if(node==0)
	{
		res[0]=dyy-2.0*dy*y[0];
		res[1]=-y[1]*dy;
	}
	else
	{
		res[0]=-dy;
		res[1]=dyy-y[0]*dy;
	}
}

void	systemf0(int node,double *f0)
{
	f0[0]=0;
	f0[1]=0;
}

}

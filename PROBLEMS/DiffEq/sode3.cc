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
	return 5;
}

int	getnpoints()
{
	return 10;
}

void	systemfun(int node, double x, double *y, double *yy,double *res)
{
	res[0]=yy[0]-cos(x);
	res[1]=yy[1]+y[0];
	res[2]=yy[2]-y[1];
	res[3]=yy[3]+y[2];
	res[4]=yy[4]-y[3];
}

void	systemder(int node, double x, double *y, double *yy,double dy,double dyy,double *res)
{
	if(node==0)
	{
		res[0]=dyy; res[1]=dy; res[2]=0; res[3]=0; res[4]=0;
	}
	else
	if(node==1)
	{
		res[0]=0; res[1]=dyy; res[2]=-dy; res[3]=0; res[4]=0;
	}
	else
	if(node==2)
	{
		res[0]=0; res[1]=0; res[2]=dyy; res[3]=dy; res[4]=0;
	}
	else
	if(node==3)
	{
		res[0]=0; res[1]=0; res[2]=0; res[3]=dyy; res[4]=-dy;
	}
	else
	{
		res[0]=0;res[1]=0;res[2]=0;res[3]=0;res[4]=dyy;
	}
}

void	systemf0(int node,double *f0)
{
	f0[0]=0;
	f0[1]=1;
	f0[2]=0;
	f0[3]=1;
	f0[4]=0;
}

}

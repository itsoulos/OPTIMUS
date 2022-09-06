# include <math.h>
/*	This is a sample file for PDE with Dirichlet boundary conditions,
 *	written in C++. The meaning of the functions is 
 *	as follows:
 *		1. getx0():  Return the left boundary of the equation on xx'.
 *		2. getx1():  Return the right boundary of the equation on xx'.
 *		3. gety0():  Return the left boundary of the equation on yy'.
 *		4. gety1():  Return the right boundary of the equation on yy'.
 *		5. getnpoints(): Return the number points in which the system
 *				will try to solve the PDE.
 *		6. getbpoints(): Return the number of boundary points.
 *		5. f0(y):  Return the  boundary condition at x=x0.
 *		6. f1(y):  Return the  boundary condition at x=x1.
 *		7. g0(x):  Return the  boundary condition at y=y0.
 *		8. g1(x):  Return the  boundary condition at y=y1.
 *		9. pde(x,y,v,x1,y1,x2,y2): Return the pde to be solvet at (x,y). The value
 *			of function is v. The first derivatives are (x1,y1) and the second
 *			ones are (x2,y2).
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

double	gety0()
{
	return 0.0;
}

double	gety1()
{
	return 1.0;
}

int	getnpoints()
{
	return 25;
}

int	getbpoints()
{
	return 50;
}

double	f0(double y)
{
	return  y  *  y * y;
}

double	f1(double y)
{
	return (1+y*y*y)*exp(-1.0);
}

double g0(double x)
{
	return x * exp(-x);
}

double g1(double x)
{
	return (x+1)*exp(-x);
}

double pde(double x, double y, double v, double x1, double y1, double x2, double y2)
{
	return x2+y2-(x+y*y*y)*exp(-x);
}

}

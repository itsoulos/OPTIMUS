# include <math.h>
/*	This is a sample file for ODE,
 *	written in C++. The meaning of the functions is 
 *	as follows:
 *		1. getx0():  Return the left boundary of the equation.
 *		2. getx1():  Return the right boundary of the equation.
 *		3. getkind():Return the kind of the equation.
 *		4. getnpoints(): Return the number points in which the system
 *				will try to solve the ODE.
 *		5. getf0():  Return the left boundary condition.
 *		6. getf1():  Return the right boundary condition.
 *		7. getff0(): Return the derivative of the left boundary condition.
 *		8. ode1ff(): Return the equation for first order equations.
 *		9. ode2ff(): Return the equation for second order equations.
 * */
extern "C"
{

double	getx0()
{
	return 0;
}

double	getx1()
{
	return 2.0;
}

int	getkind()
{
	return 2;
}

int	getnpoints()
{
	return 10;
}

double	getf0()
{
	return 0.0;
}

double	getff0()
{
	return 1.0;
}

double	getf1()
{
	return sin(1)/exp(0.2);
}

double	ode1ff(double x,double y,double yy)
{
}

double	dode1ff(double x,double y,double yy,double dy,double dyy)
{
}

double	ode2ff(double x,double y,double yy,double yyy)
{
	return yyy+yy/5.0+y+exp(-x/5.0)*cos(x)/5.0;
}

double	dode2ff(double x,double y,double yy,double yyy,double dy,double dyy,double dyyy)
{
	return dyyy+dyy/5.0+dy;
}
}

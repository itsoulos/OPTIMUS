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
	return exp(1);
}

double	getx1()
{
	return 2*exp(1);
}

int	getkind()
{
	return 2;
}

int	getnpoints()
{
	return 50;
}

double	getf0()
{
	return 0.0;
}

double	getff0()
{
	return 1.0/exp(1.0);
}

double	getf1()
{
	return log(4.0);
}

double	ode1ff(double x,double y,double yy)
{
	return yy*yy+log(y)-cos(x)*cos(x)-2.0*cos(x)-1.0-log(x+sin(x));
}

double	ode2ff(double x,double y,double yy,double yyy)
{
	return x*x*yyy+(x*yy)*(x*yy)+1.0/(log(x));
}

}

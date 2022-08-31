# include <math.h>
# include <stdio.h>
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


# define PERIOD M_PI

double sigma(double x)
{
    return x/M_PI;
}

double integral_function(double x,int n)
{
    return sigma(x) *cos(n * x) ;
}


double integral( double a, double b, int n,int n1) {
    double step = (b - a) / n;  // width of each small rectangle
    double area = 0.0;  // signed area
    for (int i = 0; i < n; i ++) {
        area+= integral_function(a+(i+0.5)*step,n1)*step;
    }
    return area;
}



double alpha_n(double x,int n)
{
  return 1.0/M_PI * integral(-M_PI,M_PI,100,n);
}


double phi(double x,int n)
{
    return M_PI/6.0;
}

double armonic(double x,int n)
{
    return alpha_n(x,n) *cos(2.0*M_PI/PERIOD*n*x-phi(x,n));
}



double gx(double x)
{
    double sum = 0.0;
    const int N1=1;
    const int N2=100;
    double a0=armonic(x,0);
    sum = a0/2.0;
    for(int i=N1;i<=N2;i++)
    {
        sum+=alpha_n(x,i)*cos(2.0*M_PI/PERIOD*i*x-phi(x,i));
    }
    return sum;
}

double	getx0()
{
	return 0;
}

double	getx1()
{
	return 1;
    double delta = 2;
    double x1 = 14.0;
	return M_PI * x1/ delta;
}

int	getkind()
{
	return 2;
}

int	getnpoints()
{
	return 100;
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
	return 1.0;
}

double	ode1ff(double x,double y,double yy)
{
	return 0.0;
}

double	dode1ff(double x,double y,double yy,double dy,double dyy)
{
}

double	ode2ff(double x,double y,double yy,double yyy)
{
    double v= yyy+y*gx(x);
    return v;
	//return yyy+100.0*y;
}

double	dode2ff(double x,double y,double yy,double yyy,double dy,double dyy,double dyyy)
{
    return dyyy+dy*gx(x);
	//return dyyy+100.0*dy;
}

}

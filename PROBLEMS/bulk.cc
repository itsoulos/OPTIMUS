# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
using namespace std;

extern "C"
{

	const double m0=9.10938356e-31;
	const double megan=0.2 * m0;
	const double mhgan=1.25 * m0;
	const double melnN=0.05 * m0;
	const double holemass=0.6 * m0;
	const double beta=1.43;
	const double egan=3.42;
	const double elnn=0.7;
	const double mhln=0.6*m0;
	const double hbar=4.135e-15;
	//the xvalue for plot
	const double xvalue = 1;
	const double epsilon0=8.8541878128e-12;
	const double epsilonR=14.6 +10.4*(1.0 - xvalue);	

double calculateElectronMass(double X)
{
	return megan + X*(melnN-megan);
}


double calculateHoleMass(double X)
{
	return mhgan +X*(mhln-mhgan);
}

double abQuantity()
{
	double m1=calculateElectronMass(xvalue);
	double m2=calculateHoleMass(xvalue);
	double meff=(m1 * m2)/(m1+m2);
	double q=1.602176634*1e-19;
	double ab= (4.0 * M_PI * epsilon0 * epsilonR * hbar * hbar)/(meff * q * q);
	return ab;
}

void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 1;
}
void    getmargins(vector<Interval> &x)
{
	x[0]=Interval(0,abQuantity());
}



double	funmin(vector<double> &x)
{
	double X=xvalue;
	double R=x[0];
	double ebulk=X*elnn+(1.0-X)*egan-beta*X*(1.0-X);
	double secondTerm = hbar * hbar /(8.0 * R * R) *(1.0/calculateElectronMass(X)+1.0/calculateHoleMass(X));	
	return ebulk + secondTerm;

}


static double dmax(double a,double b)
{
	return a>b?a:b;
}

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
	printf("R = %lf \n",x[0]);
    return QJsonObject();
}

}

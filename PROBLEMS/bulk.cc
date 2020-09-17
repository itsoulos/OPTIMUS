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
void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 2;
}
void    getmargins(vector<Interval> &x)
{
	x[0]=Interval(0,0.147);
	x[1]=Interval(0.0,1.0);
}


double calculateElectronMass(double X)
{
	return megan + X*(melnN-megan);
}


double calculateHoleMass(double X)
{
	return mhgan +X*(mhln-mhgan);
}

double	funmin(vector<double> &x)
{
	double X=x[0];
	double R=x[1];
	double ebulk=X*elnn+(1.0-X)*egan-beta*X*(1.0-X);
	double secondTerm = hbar * hbar /(8.0 * R * R) *(1.0/calculateElectronMass(X)+1.0/calculateHoleMass(X));	
	printf("terms %lf %lf \n",ebulk,secondTerm);
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
    return QJsonObject();
}

}

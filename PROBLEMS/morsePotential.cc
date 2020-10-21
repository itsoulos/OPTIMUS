# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
using namespace std;

#pragma gcc optimize("Ofast")

extern "C"{
int natoms=20;
double leftmargin=  -2.00;
double rightmargin=  2.00;
double rhoValue = 3.0;
typedef vector<double> Data;

int	getdimension()
{
    return	3 * natoms;
}
void    init(QJsonObject data)
{
    if(data.contains("natoms"))
        natoms=data["natoms"].toString().toInt();
    if(data.contains("leftmargin"))
        leftmargin=data["leftmargin"].toString().toDouble();
    if(data.contains("rightmargin"))
        rightmargin=data["rightmargin"].toString().toDouble();
    if(data.contains("rhoValue"))
	    rhoValue=data["rhoValue"].toString().toDouble();
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(leftmargin,rightmargin);
    }
}

double dpot(double r)
{
	double eps=1.0;
	double sigma=1.0;
	double d=1.0-exp(rhoValue*(1.0-r/sigma));
	return d * d-1.0;
}

double gpot(double r)
{
	double eps=1.0;
	double sigma=1.0;
	double d=1.0-exp(rhoValue*(1.0-r/sigma));
	return  2.0 * d * (-exp(rhoValue*(1.0-r/sigma)*(-1.0/sigma)));
}

double	funmin(Data &x)
{
    double value=0.0;
           for(int i=1;i<=natoms;i++)
           {
		   int i31 = 3*i;
                   for(int j=i+1;j<=natoms;j++)
                   {
			   int i32 = 3*j;
			   double rij = sqrt( (x[i31-2-1] - x[i32-2-1])*(x[i31-2-1] - x[i32-2-1])+
				       	      (x[i31-1-1] - x[i32-1-1])*(x[i31-1-1] - x[i32-1-1])+
					      (x[i31-1] - x[i32-1])*(x[i31-1] - x[i32-1]) );
                           value=value+dpot(rij);
                   }
           }
           return  value;

}
static double dmax(double a,double b){return a>b?a:b;}

void    granal1(vector<double> &x,vector<double> &g)
{




    for(int i=1;i<=natoms;i++)
    {
	    int i31=3*i; 
	    int idim=3 * i;
	    g[idim-1]=0.0;
	    g[idim-1-1]=0.0;
	    g[idim-2-1]=0.0;
	    for(int j=1;j<=natoms;j++)
	    {
		    int i32=3*j;
		    if(j!=i)
		    {
			double rij = sqrt( (x[i31-2-1] - x[i32-2-1])*(x[i31-2-1] - x[i32-2-1])+
                                              (x[i31-1-1] - x[i32-1-1])*(x[i31-1-1] - x[i32-1-1])+
                                              (x[i31-1] - x[i32-1])*(x[i31-1] - x[i32-1]) );
			g[idim-2-1]+=gpot(rij)* (x[i31-2-1] - x[i32-2-1])/rij;
			g[idim-1-1]+=gpot(rij)*(x[i31-1-1] - x[i32-1-1])/rij;
			g[idim-1]+=gpot(rij)*(x[i31-1] - x[i32-1])/rij;
		    }
	    }
    }

}

QJsonObject    done(Data &x)
{
    return QJsonObject();
}
void	granal(vector<double> &x,vector<double> &g)
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

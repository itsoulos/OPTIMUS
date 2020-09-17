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
    double  eps=1.0;
    double  sig =1.0;
    double  eps4=4.0 *eps;
    //double sbyr6=pow(sig/r,6.0);
    double sbyr6=sig*sig*sig*sig*sig*sig/r/r/r/r/r/r;
    return eps4 * sbyr6*(sbyr6-1.0);
}

double gpot(double r)
{
    double  eps=1.0;
    double  sig =1.0;
    double  eps4=4.0 *eps;
    double sl = 0.0;

    //double sbyr7 = pow((sig/r),7.0);
    //double sbyr6 = pow((sig/r),6.0);
    double sbyr6=sig*sig*sig*sig*sig*sig/r/r/r/r/r/r;
    double sbyr7=sbyr6*sig/r;

     return  eps4*sbyr6*(-12.0*sbyr7 + 6.0*(sig/r))+sl/(r*r);

}

double	funmin(Data &x)
{
    //double *x = &x1[0];
    double value=0.0;
    //Data xx; xx.resize(natoms);
    //Data yy; yy.resize(natoms);
    //Data zz; zz.resize(natoms);
    /*
           for(int i=1;i<=natoms;i++)
           {
                   int i3=3 * i;
                   xx[i-1]=x[i3-2-1];
                   yy[i-1]=x[i3-1-1];
                   zz[i-1]=x[i3-1];
           }*/
           for(int i=1;i<=natoms;i++)
           {
		   int i31 = 3*i;
                   for(int j=i+1;j<=natoms;j++)
                   {
			   int i32 = 3*j;
                           //double dx=xx[i-1]-xx[j-1];
                           //double dy=yy[i-1]-yy[j-1];
                           //double dz=zz[i-1]-zz[j-1];
                           //double rij=sqrt(dx*dx+dy*dy+dz*dz);
			   double rij = sqrt( (x[i31-2-1] - x[i32-2-1])*(x[i31-2-1] - x[i32-2-1])+
				       	      (x[i31-1-1] - x[i32-1-1])*(x[i31-1-1] - x[i32-1-1])+
					      (x[i31-1] - x[i32-1])*(x[i31-1] - x[i32-1]) );
                           value=value+dpot(rij);
                   }
           }
           return  value;

}
static double dmax(double a,double b){return a>b?a:b;}

void    granal(vector<double> &x,vector<double> &g)
{




    //Data xx; xx.resize(natoms);
    //Data yy; yy.resize(natoms);
    //Data zz; zz.resize(natoms);
    /*
    for(int i=1;i<=natoms;i++)
    {
       int i3=3 * i;
       xx[i-1]=x[i3-2-1];
       yy[i-1]=x[i3-1-1];
       zz[i-1]=x[i3-1];
     }*/
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
			//double dx=xx[i-1]-xx[j-1];
                        //double dy=yy[i-1]-yy[j-1];
                        //double dz=zz[i-1]-zz[j-1];
			//double rij=sqrt(dx*dx+dy*dy+dz*dz);
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
}

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
double leftmargin=  -1.00;
double rightmargin=  1.00;
double	A=3264.7;
double  B=95.373;
double l1=3.2394;
double l2=1.3258;
double gammaValue=0.33675;
double n = 22.956;
double d = 2.0417;
double h = 0.0000;
double c = 4.8381;
double R = 3.0;
double D = 0.2;

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



double VR(double rij)
{
	return A * exp (-l1 * rij);
}

double VA(double rij)
{
	return B * exp(-l2 * rij);
}

double g(double theta)
{
	return 1.0 + c*c/(d*d)-(c*c)/(d*d+(h-cos(theta)*(h-cos(theta))));
}

double fc(double r)
{
//	printf("r %lf R %lf D %lf \n",r,R,D);
	if(r<R-D) return 1.0;
	else
	if(r>=(R-D) && r<=(R+D)) return 0.5-0.5 * sin(M_PI *(r-R)/(2.0*D));
	else return 0.0;	
}



double dpot(double r)
{
    double  eps=1.0;
    double  sig =1.0;
    double  eps4=4.0 *eps;
    double sbyr6=pow(sig/r,6.0);
    return eps4 * sbyr6*(sbyr6-1.0);
}


double gpot(double r)
{

	double eps = 1.0, sig = 1.0;
        double eps4 = 4*eps;
        double sl = 0.0;

        double sbyr7 = pow((sig/r),7);
        double sbyr6 = pow((sig/r),6);
        return eps4*sbyr6*(-12.0*sbyr7 + 6.0*(sig/r))+sl/(r*r);

}

double rvalue(Data &x,int i,int j)
{
	int i31=3 * i;
	int i32=3 * j;
	   double rij = sqrt( (x[i31-2-1] - x[i32-2-1])*(x[i31-2-1] - x[i32-2-1])+
				       	      (x[i31-1-1] - x[i32-1-1])*(x[i31-1-1] - x[i32-1-1])+
					      (x[i31-1] - x[i32-1])*(x[i31-1] - x[i32-1]) );
	   return rij;
}

double costheta(Data &x,int i,int j,int k)
{

	return (pow(rvalue(x,i,j),2.0)+pow(rvalue(x,i,k),2.0)-pow(rvalue(x,j,k),2.0))/(2.0*rvalue(x,i,j)*rvalue(x,i,k));
}

double gfunction(double cosv)
{
	return 1.0+c*c/(d*d)-c*c/(d*d+pow(h-cosv,2.0));
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

			   double ksiValue=0.0;
			   double biValue=0.0;
			   for(int k=1;k<=natoms;k++)
			   {
				   if(k==i) continue;
				   if(k>=j) continue;
				   double cosv=costheta(x,i,j,k);
				   ksiValue+=fc(rvalue(x,i,k))*gfunction(cosv);
				   
			   }
			   biValue=pow(1.0+pow(gammaValue*ksiValue,n),-1.0/(2.0*n));
		   	value+=fc(rij)*(VR(rij)+biValue*VA(rij));	   
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

			double gp = gpot(rij);
			g[idim-2-1]+=gp* (x[i31-2-1] - x[i32-2-1])/rij;
			g[idim-1-1]+=gp*(x[i31-1-1] - x[i32-1-1])/rij;
			g[idim-1]+=gp*(x[i31-1] - x[i32-1])/rij;
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

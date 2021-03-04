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

 double s0=403;
 double si=s0;
 double jerk=s0;
 double l=3.025;
 double di=2.3;
 double d0=2.3;
 double d1 = 2.3;
 double d2 = 0.5;
 double d3= 0.5 ; 
 double d4= 4.0 ;
 double d5 = 4.0;
 double d6= 4.0;
 double d7 = 2.3 ;
 double t = 0;
 double k0=0.0125;
 double ki = 0.01050223;
 double k_a0=0.009;
 double k_ai=0.007335;
 double Result = 1 ;
 double h=(1/(362.46*l))*pow(10,6) ;
 double ttime=1440;
 double i=0;
 double tint=1;
 double a=0;
 double b=1;
 double Max=s0;
 int n=0;
 double datapoint30=689.5;
 double datapoint60=613.5;
 double datapoint90=529.2;
 double datapoint1439=403.0;
double Array[4];
 
 double max( double jerk);
 double k( double i);
 double C( double i, double di, double ki, double k_ai, double si);
 double d( double i, double d0, double k_ai);
 double M( double k0, double k_a0);


 double f_C( double i, double di, double ki, double k_ai, double si){
 double C=h*di*(k_ai/(-k_ai+ki))*(-exp(-ki*i)+exp(-k_ai*i))+si*exp(-ki*i) ;
return C;
}


 double f_k_a( double i){
 double k_a;
k_a=k_a0;
k_a=k_a0*cbrt((d0/di));
if(di<0.0001){
k_a=0;	
}
return k_a;
}


 double d( double i, double d0, double k_ai){
	
 double d=d0*exp(-k_ai*i);
return d;
}


 double max( double jerk){

if(Max<jerk)
{Max=jerk;}
return Max;
}

 double k( double i){
 double k;
k=pow((max(jerk)/(s0)),1)*k0;

return k;
}



 double f_s( double i){
 double s;
if(i==0){
s=f_C(0,di,ki,k_ai,si);}
else
s=f_C(1,di,ki,k_ai,si);
return s;
}



void    init(QJsonObject data)
{

}

int	getdimension()
{
	return 2;
}
void    getmargins(vector<Interval> &x)
{
	x[0]=Interval(log(2.0)/122.0,log(2.0)/13.0);
	x[1]=Interval(log(2.0)/173.0,log(2.0)/73.0);
	
}


//f(x)
double	funmin(vector<double> &x)
{
	k0=x[0];
    k_a0=x[1];
    double sum=0.0;
 double M1;  double M2;  double M3;
 double M4;
for(i=0;i<(ttime/tint);i++)
{
//Reset on the 12th hour mark	
//if(i==540) 
//{Max=C(1,di,ki,k_ai,si);
//}
k_ai=f_k_a(i);
b=i/1440;
	
if(tint*i==1260)
{Max=f_C(1,di,ki,k_ai,si);
}

si=f_s(i);
ki=k(i);
//Add the dosages when the time is right
if(i==0)
{di=d(i,d0,k_ai);}

else if(i==180)
{di=d(1,di,k_ai);
di += d1; 
}
else if(i==360)
{
di=d(1,di,k_ai);
di +=d2;}
else if(i==540)
{di=d(1,di,k_ai);

di +=d3;
}
else if(i==720)
{di=d(1,di,k_ai);
di +=d4;
}
else if(i==900)
{di=d(1,di,k_ai);
di +=d5;
}
else if(i==1080)
{di=d(1,di,k_ai);
di +=d6;
}
else if(i==1260)
{di=d(1,di,k_ai);
di +=d7;
}
else 
{di=d(1,di,k_ai);
}
//Overwrite values for jerk and Max
jerk=f_C(1,di,ki,k_ai,si);
max(jerk); 
if(i==30)
{M1=si;
Array[0]=M1;
sum+=pow(M1-datapoint30,2.0);
}
if(i==60)
{
M2=si; 
Array[1]=M2;
sum+=pow(M2-datapoint60,2.0);
}
if(i==90)
{M3=si; 
Array[2]=M3;
    sum+=pow(M3-datapoint90,2.0);
}
if(i==1439)
{M4=si; 
Array[3]=M4;
    sum+=pow(M4-datapoint1439,2.0);
}
}
return sum;
}

static double dmax(double a,double b)
{
	return a>b?a:b;
}
//f'(x)
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
	printf("Final values %lf %lf \n",x[0],x[1]);
	printf("X     values \n");
	for(int i=0;i<4;i++) printf(" %10.4lf ",Array[i]);
	printf("\n");
    return QJsonObject();
}

}

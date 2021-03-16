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

//Constant Variables
const   double s0=403;
const   double l=3.025;
const   double d1 = 2.3;
const   double d2 = 0.5;
const   double d3= 0.5 ;
const   double d4= 4.0 ;
const   double d5 = 4.0;
const   double d6= 4.0;
const   double d7 = 2.3 ;
const   double h=(1/(362.46*l))*pow(10,6) ;
const   double tint=1;
const   double ttime=1440;

double datapoint30=689.50;
double datapoint60=613.50;
double datapoint90=529.20;
double datapoint1439=403.00;

//Non constant Global Variables
  double d0=2.3; //Chosen at the beginning of the programm by the user then constant
  double k0=0.0125;//As above
  double k_a0=0.009; //As above

  double i=0;//Being used within the for loop in the C function
  double Max=s0;//Being used within more than one functions
  double Array [4];
int n=0;
  double si=s0;//Being used within more than one functions
  double jerk=s0; //Being used within more than one functions
  double di=d0;//Being used within more than one functions
  double k_ai=k_a0;//Being used within more than one functions
  double ki = k0;//Being used within more than one functions
//Functions (Cannot be local within C++ framnework)
  double max(  double jerk);

  double k(  double i);

  double C(  double i,  double di,  double ki,  double k_ai,  double si);

  double k_a(  double i);

  double d(  double i,  double d0,  double k_ai);

  double s(  double i);

  double M(  double k0,  double k_a0);



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
	x[1]=Interval(log(2.0)/173.0,log(2.0)/60.0);
	
}


  double C(  double i,  double di,  double ki,  double k_ai,  double si){
  double C=h*di*(k_ai/(-k_ai+ki))*(-exp(-ki*i)+exp(-k_ai*i))+si*exp(-ki*i) ;
return C;
}



  double k_a(  double i){
  double k_a;
k_a=k_a0;
k_a=k_a0*cbrt((d0/di));
if(di<0.0001){
k_a=0;
}
return k_a;
}


  double d(  double i,  double d0,  double k_ai){

  double d=d0*exp(-k_ai*i);
return d;
}


  double max(  double jerk){

if(Max<jerk)
{Max=jerk;}
return Max;
}

  double k(  double i){
  double k;
k=pow((max(jerk)/(s0)),1)*k0;

return k;
}

  double s(  double i){
  double s;
if(i==0){
s=C(0,di,ki,k_ai,si);}
else
s=C(1,di,ki,k_ai,si);
return s;
}

//f(x)
double	funmin(vector<double> &x)
{
    double k0=x[0];
    double k_a0=x[1];
    if(x[1]>=x[0]) return 1e+10;
    double sum=0.0;



      double M1;   double M2;   double M3;
      double M4;
    si=s0;//Reinitializing to make sure the program is running with the proper value
    jerk=s0; //Similarly
    di=d0;//Similarly
    k_ai=k_a0;//Similarly
    ki = k0;//Similarly
    Max=s0;//Similarly
    for(i=0;i<(ttime/tint);i++)
    {
    //Reset on the 12th hour mark
    //if(i==540)
    //{Max=C(1,di,ki,k_ai,si);
    //}
    k_ai=k_a(i);


    if(tint*i==1260)
    {Max=C(1,di,ki,k_ai,si);
    }

    si=s(i);
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
    jerk=C(1,di,ki,k_ai,si);
    max(jerk);
    if(i==30)
    {M1=si;Array[0]=M1;
        sum+=pow(M1-datapoint30,2.0);
    }
    if(i==60)
    {
    M2=si; Array[1]=M2;
    sum+=pow(M2-datapoint60,2.0);
    }
    if(i==90)
    {M3=si; Array[2]=M3;
    sum+=pow(M3-datapoint90,2.0);
    }
    if(i==1439)
    {M4=si; Array[3]=M4;
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

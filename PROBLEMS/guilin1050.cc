# include <math.h>
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

double *c;
int      *K;

# define DIMENSION	10
# define MINIMA		50

static int min_div(int N)
{
        int s=0;
        for(int i=2;i<=N/2;i++)
                if(N%i==0) return i;
        return s;
}

static void split(int N,int K,int *n)
{
        int i=0;
        while(1)
        {
                if(i==K-1) {n[i]=N;return;}
                n[i]=min_div(N);
                if(n[i]==0) break;
                N=N/n[i];
                i=i+1;
        }
        for(int j=i;j<K-1;j++)
                n[j]=1;
        n[K-1]=N;
}

int	getdimension()
{
	return DIMENSION;
}

void    init(QJsonObject data)
{

	   K=new int[DIMENSION];
           split(MINIMA,DIMENSION,K);
	   c=new double[DIMENSION];
	   c[0]=1.0;
	   for(int i=1;i<DIMENSION;i++)
	     c[i]=c[i-1]+0.5;
}
void    getmargins(vector<Interval> &x)
{
        for(int i=0;i<x.size();i++)
                x[i]=Interval(0,1);
}
double	funmin(vector<double> &x)
{
	double sum=3.0;
	for(int i=0;i<DIMENSION;i++)
		  sum+=c[i]*(x[i]+9.0)/(x[i]+10.0)*sin(M_PI/(1.0-x[i]+1.0/(2.0*K[i])));
	return sum;

}

void    granal(vector<double> &x,vector<double> &g)
{
	 for(int i=0;i<DIMENSION;i++)
        {
                double a,b,aa,bb;
                a=c[i]*(x[i]+9.0)/(x[i]+10.0);
                b=sin(M_PI/(1.0-x[i]+1.0/(2.0*K[i])));
                aa=c[i]*(1.0)/pow(x[i]+10.0,2.0);
                bb=M_PI/pow((1.0-x[i]+1.0/(2.0*K[i])),2.0)*
                        cos(M_PI/(1.0-x[i]+1.0/(2.0*K[i])));
                g[i]=a*bb+b*aa;
        }
}

}

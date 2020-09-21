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


/*	Cosine Mixture
 * */
extern "C"
{

int dimension=64;
int	getdimension()
{
    return dimension;
}

void    init(QJsonObject data)
{
    if(data.contains("natoms"))
	dimension=data["natoms"].toString().toInt();
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(-1.0,1.0);
    }
}

double	funmin(vector<double> &x)
{
	double s1=0.0,s2=0.0;
	for(int i=0;i<dimension;i++)
	{
		s1=s1+cos(5.0 * M_PI * x[i]);
		s2=s2+x[i]*x[i];
	}	
	return s2-0.1 * s1;
}

void    granal(vector<double> &x,vector<double> &g)
{
	for(int i=0;i<dimension;i++)
	{
		g[i]=2.0 * x[i]+0.1 * 5.0 * M_PI * sin(5.0 *M_PI *x[i]);
	}
}

}

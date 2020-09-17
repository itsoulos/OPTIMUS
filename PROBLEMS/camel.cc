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
void    init(QJsonObject data)
{

}

void    getmargins(vector<Interval> &x)
{
        for(int i=0;i<x.size();i++)
                x[i]=Interval(-5,5);
}

int	getdimension()
{
	return 2;
}

double	funmin(vector<double> &x)
{
 double x1=x[0],x2=x[1];
 return 4*x1*x1-2.1*x1*x1*x1*x1+x1*x1*x1*x1*x1*x1/3.0+x1*x2-4*x2*x2+4*x2*x2*x2*x2;
}

void    granal(vector<double> &x,vector<double> &g)
{
 double x1=x[0],x2=x[1];
 g[0]=8*x1-8.4*x1*x1*x1+2*x1*x1*x1*x1*x1+x2;
 g[1]=x1-8*x2+16*x2*x2*x2;
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

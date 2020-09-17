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

int	getdimension()
{
	return 2;
}
void    getmargins(vector<Interval> &x)
{
        for(int i=0;i<x.size();i++)
                x[i]=Interval(-1,1);
}

/*
interval ifunmin(vector<interval> &x)
{
        return abs(x[0]*x[0])+abs(x[1]*x[1])-cos(18.0 * x[0])-cos(18.0*x[1]);
}*/

double	funmin(vector<double> &x)
{
   return abs(x[0]*x[0])+abs(x[1]*x[1])-cos(18.0*x[0])-cos(18.0*x[1]);
}

void    granal(vector<double> &x,vector<double> &g)
{
      g[0]=2.0*x[0]+18.0*sin(18.0*x[0]);
      g[1]=2.0*x[1]+18.0*sin(18.0*x[1]);
}

QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}

}

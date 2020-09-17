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
	return 4;
}


void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<x.size();i++)
    {
    x[i]=Interval(0.0,10.0);
    }
}

double	funmin(vector<double> &x)
{
	double a[10][4];
	double c[10];
	a[0][0]=a[0][1]=a[0][2]=a[0][3]=4;
        a[1][0]=a[1][1]=a[1][2]=a[1][3]=1;
        a[2][0]=a[2][1]=a[2][2]=a[2][3]=8;
        a[3][0]=a[3][1]=a[3][2]=a[3][3]=6;
        a[4][0]=3;a[4][1]=7;a[4][2]=3;a[4][3]=7;
        a[5][0]=2;a[5][1]=9;a[5][2]=2;a[5][3]=9;
        a[6][0]=5;a[6][1]=5;a[6][2]=3;a[6][3]=3;
        a[7][0]=8;a[7][1]=1;a[7][2]=8;a[7][3]=1;
        a[8][0]=6;a[8][1]=2;a[8][2]=6;a[8][3]=2;
        a[9][0]=7;a[9][1]=3.6;a[9][2]=7;a[9][3]=3.6;
        c[0]=0.1;c[1]=0.2;c[2]=0.2;c[3]=0.4;c[4]=0.4;
        c[5]=0.6;c[6]=0.3;c[7]=0.7;c[8]=0.5;c[9]=0.5;
        double s=0.0;
        for(int i=0;i<10;i++)
        {
                double ss=0.0;
                for(int j=0;j<4;j++) ss+=pow(x[j]-a[i][j],2.0);
                s=s+1.0/(ss+c[i]);
        }
        s=-s;
        return s;
}

void    granal(vector<double> &x,vector<double> &g)
{
	double a[10][4];
	double c[10];
	a[0][0]=a[0][1]=a[0][2]=a[0][3]=4;
        a[1][0]=a[1][1]=a[1][2]=a[1][3]=1;
        a[2][0]=a[2][1]=a[2][2]=a[2][3]=8;
        a[3][0]=a[3][1]=a[3][2]=a[3][3]=6;
        a[4][0]=3;a[4][1]=7;a[4][2]=3;a[4][3]=7;
        a[5][0]=2;a[5][1]=9;a[5][2]=2;a[5][3]=9;
        a[6][0]=5;a[6][1]=5;a[6][2]=3;a[6][3]=3;
        a[7][0]=8;a[7][1]=1;a[7][2]=8;a[7][3]=1;
        a[8][0]=6;a[8][1]=2;a[8][2]=6;a[8][3]=2;
        a[9][0]=7;a[9][1]=3.6;a[9][2]=7;a[9][3]=3.6;
        c[0]=0.1;c[1]=0.2;c[2]=0.2;c[3]=0.4;c[4]=0.4;
        c[5]=0.6;c[6]=0.3;c[7]=0.7;c[8]=0.5;c[9]=0.5;
	for(int j=0;j<4;j++)
        {
		double s=0.0;
		for(int  i=0;i<10;i++)
		{
			double ss=0.0;
			for(int k=0;k<4;k++) ss+=pow(x[k]-a[i][k],2.0);
			ss+=c[i];
			s=s+pow(ss,-2.0)*2.0*(x[j]-a[i][j]);
		}
		g[j]=s;
        }
}
QJsonObject    done(vector<double> &x)
{
    return QJsonObject();
}
}

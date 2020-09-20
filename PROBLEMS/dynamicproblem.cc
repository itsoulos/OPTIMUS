# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


#pragma gcc optimize("Ofast")

extern "C"{
    
    
    
typedef vector<double> Data;

int natoms=20;
double leftmargin=  -2.00;
double rightmargin=  2.00;


#define sigma 1.0
#define STEPS 1000000
vector<Data> reactions,velocities;


void init_reactions();
void init_velocities();

double potential( Data &,Data &);
double calculate_field(Data &);
void update_velocities();
void update_positions(Data &pos, double step=0.01);

void init_reactions(){
    for(unsigned i = 0; i < natoms; i++){
        for(unsigned j = 0; j < 3; j++){
            reactions[i][j] = 0;
        }
    }
}

void init_velocities(){
    for(unsigned i = 0; i < natoms; i++){
        for(unsigned j = 0; j < 3; j++){
            velocities[i][j] = 0;
        }
    }
}

double potential(Data &posA,Data &posB){
    double r_squared = 0;
    const double eps=1.0;
    for(unsigned i = 0; i < 3; i++)
        r_squared += (posA[i] - posB[i]) * (posA[i] - posB[i]);
    double sbyr6=sigma*sigma/r_squared;
    sbyr6 = sbyr6*sbyr6*sbyr6;
    return 4 * eps * sbyr6*(sbyr6-1.0);
}

double calculate_field(Data &pos){
    double minPot = 1e+6;
    double maxPot = -1e+6;
    double sumPot = 0;
    Data posA,posB;
    posA.resize(3);
    posB.resize(3);
    for(unsigned i = 0; i < natoms-1; i++){
        for(unsigned j = i + 1; j < natoms; j++){
            for(int l=0;l<3;l++)
            {
                posA[l]=pos[i*3+l];
                posB[l]=pos[j*3+l];
            }
            double pot = potential(posA,posB);
            minPot = (pot<minPot)? pot : minPot;
            maxPot = (pot>maxPot)? pot : maxPot;
            sumPot += pot;
            double d = 0.0;
            for(unsigned k = 0; k < 3; k++){
                d+=(pos[i*3+k]-pos[j*3+k])*(pos[i*3+k]-pos[j*3+k]);
            }
            d = sqrt(d);
            for(unsigned k = 0; k < 3; k++){
                reactions[i][k] += sgn(d-1.1225) * (pot) * (pos[i*3+k]-pos[j*3+k]) / d;
                reactions[j][k] -= sgn(d-1.1225) * (pot) * (pos[i*3+k]-pos[j*3+k]) / d;
            }
        }
    }

    for(unsigned i = 0; i < natoms-1; i++){
        for(unsigned j = i + 1; j < natoms; j++){
            double d = 0;
            for(unsigned k = 0; k < 3; k++){
                d+=(pos[i*3+k]-pos[j*3+k])*(pos[i*3+k]-pos[j*3+k]);
            }
            d = sqrt(d);
            for(unsigned k = 0; k < 3; k++){
                reactions[i][k] += sgn(d-1.1225) * minPot * (pos[i*3+k]-pos[j*3+k]) / d;
                reactions[j][k] -= sgn(d-1.1225) * minPot * (pos[i*3+k]-pos[j*3+k]) / d;
            }
        }
    }
    return sumPot;
}

void update_velocities(){
    for(unsigned i = 0; i < natoms; i++){
        for(unsigned j = 0; j < 3; j++){
            velocities[i][j] = (0.1*velocities[i][j] + 0.9*reactions[i][j]/natoms);
        }
    }
}

void update_positions(Data &pos,  double step){
    for(unsigned i = 0; i < natoms; i++){
        for(unsigned j = 0; j < 3; j++){
            pos[i*3+j] += step*velocities[i][j];
        }
    }
}



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
    reactions.resize(natoms);
    velocities.resize(natoms);
    for(int i=0;i<natoms;i++)
    {
        reactions[i].resize(3);
        velocities[i].resize(3);
    }
    init_reactions();
    init_velocities();
}

void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(leftmargin,rightmargin);
    }
}



double	funmin(Data &x)
{
    double f;
    init_reactions();
    init_velocities();
    for(int iters=1;iters<=1000;iters++)
    {
        f=calculate_field(x);
        update_velocities();
        update_positions(x,0.001);
        init_reactions();
    }
    return f;
}
static double dmax(double a,double b){return a>b?a:b;}

void    granal(vector<double> &x,vector<double> &g)
{

   for(int i=0;i<natoms;i++)
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

QJsonObject    done(Data &x)
{
    return QJsonObject();
}
}

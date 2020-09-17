#include "kmeans.h"
# include <math.h>
kmeans::kmeans(Dataset *d,int nteams)
{
    int i;
    dataset=d;
    team=nteams;
    center.resize(team);
    for(i=0;i<team;i++)
       center[i].resize(dataset->getdimension());
    member.resize(dataset->getpatterns());
    for(i=0;i<member.size();i++)
        member[i]=-1;
}

double  kmeans::distance(Data &x, Data &y)
{
    double s=0.0;
    int i;
    for(i=0;i<x.size();i++)
        s+=pow(x[i]-y[i],2.0);
    return sqrt(s);
}

void    kmeans::runAlgorithm()
{
    int i;
    initCenters();
    QVector<Data> copyCenters;
    while(true)
    {
        copyCenters=center;
        for(i=0;i<member.size();i++)
        {
            Data x=dataset->getpoint(i);
            int t=nearestTeam(x);
            member[i]=t;
        }
        updateCenters();
        double totalDistance=0.0;
        for(i=0;i<center.size();i++)
            totalDistance+=distance(center[i],copyCenters[i]);
        if(totalDistance<1e-5) break;
    }
}

void    kmeans::initCenters()
{
    int i;
    //srand(0);
    for(i=0;i<member.size();i++)
    {
        member[i]=i%team;//rand() % team;
    }
    updateCenters();
}

int     kmeans::nearestTeam(Data &x)
{
    int i;
    double minDist=1e+100;
    int imin=-1;
    for(i=0;i<team;i++)
    {
        double dist=distance(x,center[i]);
        if(dist<minDist)
        {
            minDist=dist;
            imin=i;
        }
    }
    return imin;
}

void    kmeans::updateCenters()
{
 int i,j;
 for(i=0;i<team;i++)
 {
     for(j=0;j<center[i].size();j++)
         center[i][j]=0.0;
 }
 teamMembers.resize(team);
 teamMembers.fill(0);
 for(i=0;i<member.size();i++)
 {
     teamMembers[member[i]]++;
 }
 for(i=0;i<member.size();i++)
 {
     Data x=dataset->getpoint(i);
     int whatTeam=member[i];
     for(j=0;j<x.size();j++)
     {
         center[whatTeam][j]+=x[j]/teamMembers[whatTeam];
     }
 }
}


QVector<Data> kmeans::getCenters()
{
    return center;
}

Data          kmeans::getVariances()
{
    int i,j;
    Data variance;

    for(i=0;i<center.size();i++)
    {
        variance<<0.0;
    }
    double total_var=0.0;
    for(i=0;i<center.size();i++)
    {
        double sum=0.0;
        for(j=0;j<dataset->getpatterns();j++)
        {
            if(member[j]==i)
            {
                Data x=dataset->getpoint(j);
                sum+=distance(center[i],x);
            }
        }
        //an exei mono ena melos mia omada
        if(teamMembers[i]==1)
             variance[i]=sqrt(1.0/(teamMembers[i]) * sum);
        else
        variance[i]=sqrt(1.0/(teamMembers[i]-1.0) * sum);
        total_var+=variance[i];
    }
    //gia na apofygoume poly xamiles times
    for(i=0;i<center.size();i++)
        variance[i]=total_var;
    return variance;
}

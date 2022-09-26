#include "multistart.h"

Multistart::Multistart(Problem *p)
    :Optimizer(p)
{
    addParameter("multistart_samples","25","Number of multistart samples");
    addParameter("multistart_maxiterations","200","Number of maximum iterations");
}

bool Multistart::terminated()
{
    int multistart_maxiterations=params["multistart_maxiterations"].toString().toInt();
    double fmin=fabs(1.0+mbesty);
    if(iteration<=1) {x1=0;x2=0.0;}
    x1+=fmin;
    x2+=fmin * fmin;
    variance=x2/(iteration+1)-(x1/(iteration+1))*(x1/(iteration+1));
    variance=fabs(variance);
    if(mbesty<oldBesty)
    {
        oldBesty=mbesty;
        stopat=variance/2.0;
    }
    if(fabs(stopat)<1e-8) stopat=variance/2.0;
    return (iteration>=multistart_maxiterations || (variance<=stopat && iteration>=10));
}

double mdelta(double a,double r0,double r)
{
        double b=a/r0;
        if(r>=0 && r<=r0) return a-b*r;
        return 0.0;
}

vector<Data> samplex;

void    Multistart::step()
{
    int Multistart_samples=params["multistart_samples"].toString().toInt();
    ++iteration;
	QString sampling="repulsion";
    sampling = "uniform";
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<Multistart_samples;i++)
    {
        Data trialx=myProblem->getRandomPoint();
    	if(sampling=="repulsion")
    {
        const double alpha=0.1;
        const double rho0=0.25;
        trialx=myProblem->getRandomPoint();
        for(int j=0;j<trialx.size();j++)
        {
           double y=trialx[j];
           for(int k=0;k<samplex.size();k++)
           {
              Data z=trialx;
              z=samplex[k];
              for(int l=0;l<z.size();l++)
              {
                  double r=getDistance(trialx,z);
                  double u=y+mdelta(alpha,rho0,r)*(y-z[l])/r;
                  trialx[j]=u;
               }
            }

         }
	samplex.push_back(trialx);
    }
        double y=localSearch(trialx);
#pragma omp critical
{
        if(y<mbesty)
            mbesty=y;
}
 }
    printf("Multistart. Iteration:%5d Global minimum: %20.10lg variance: %lf stopat: %lf\n",
           iteration,myProblem->getBesty(),variance,stopat);
}

void Multistart::init()
{
    x1=0.0;
    x2=0.0;
    iteration=0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;
    mbesty=1e+100;
    trialx.resize(myProblem->getDimension());
	samplex.clear();
}

void Multistart::done()
{

}

void Multistart::Solve()
{
    Optimizer::Solve();
}

Multistart::~Multistart()
{

}

extern "C" MULTISTART_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Multistart(p);
}

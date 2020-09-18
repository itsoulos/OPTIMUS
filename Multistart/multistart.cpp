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
    return (iteration>=multistart_maxiterations || (variance<=stopat && iteration>=20));
}

void    Multistart::step()
{
    int Multistart_samples=params["multistart_samples"].toString().toInt();
    ++iteration;
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<Multistart_samples;i++)
    {
        Tolmin mTolmin(myProblem);
        Data trialx=myProblem->getRandomPoint();
        double y=mTolmin.Solve(trialx);
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
}

void Multistart::done()
{

}

void Multistart::setSettings(QJsonObject settings)
{
    if(settings.contains("multistart_samples"))
    addParameter("multistart_samples",settings["multistart_samples"].toString() ,"Number of multistart samples");
    if(settings.contains("multistart_maxiterations"))
    addParameter("multistart_maxiterations",settings["multistart_maxiterations"].toString(),
            "Number of maximum iterations");
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

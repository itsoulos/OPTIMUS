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
    double fmin=fabs(myProblem->getBesty());
    x1+=fmin;
    x2+=fmin * fmin;
    variance=x2/iteration-(x1/iteration)*(x1/iteration);
    variance=fabs(variance);
    if(myProblem->getBesty()<oldBesty)
    {
        oldBesty=myProblem->getBesty();
        stopat=variance/2.0;
    }
    if(fabs(stopat)<1e-8) stopat=variance/2.0;
    return (iteration>=multistart_maxiterations || (variance<=stopat && iteration>=20));
}

void    Multistart::step()
{
    ++iteration;
    trialx=myProblem->getRandomPoint();
    double y=tolmin(trialx,myProblem,2001);
    printf("Multistart. Iteration:%5d Global minimum: %20.10lg\n",iteration,myProblem->getBesty());
}

void Multistart::init()
{
    x1=0.0;
    x2=0.0;
    iteration=0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;
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

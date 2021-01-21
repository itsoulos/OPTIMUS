#include "mincenter.h"

MinCenter::MinCenter(Problem *p)
    :Optimizer(p)
{
    addParameter("mincenter_samples","500","Number of samples taken");
    addParameter("mincenter_centers","50","Number of centers");
    addParameter("mincenter_iterations","200","Number of iterations");
}


bool    MinCenter::terminated()
{
    return currentIteration>=iterations;
}

void    MinCenter::step()
{
    ++currentIteration;
    for (unsigned i = 0; i < allSamples.size(); i++)
        allSamples[i] = myProblem->getRandomPoint();
     kmeans->getNewSamples(allSamples);
     kmeans->run();
}

void    MinCenter::init()
{
    currentIteration=0;
    iterations=params["mincenter_iterations"].toString().toInt();
    centers=params["mincenter_centers"].toString().toInt();
    samples=params["mincenter_samples"].toString().toInt();
    kmeans=new KMeans();
    kmeans->setClusters(centers);
    allSamples.resize(samples);
    for (int i = 0; i < samples; i++)
            allSamples[i] = myProblem->getRandomPoint();
    kmeans->init(allSamples);

}

void    MinCenter::done()
{
    Data bestx;
    double mbesty=1e+100;

    allmeans = kmeans->getMeans();

#pragma omp parallel for num_threads(threads)
            for (unsigned int i=0;i<allmeans.size();i++){
                Point p=allmeans[i];
                Tolmin mTolmin(myProblem);
                Data trialx = p.data_;
                double y=mTolmin.Solve(trialx);
#pragma omp critical
{
                if(y<mbesty)
                {
                    bestx=trialx;
                    mbesty=y;
                }
}
            }
     mbesty=myProblem->funmin(bestx);
    if(kmeans!=0) delete kmeans;
}

MinCenter::~MinCenter()
{

}

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MinCenter(p);
}

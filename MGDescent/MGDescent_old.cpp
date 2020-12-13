#include "MGDescent.h"
MGDescent::MGDescent(Problem *p)
    : Optimizer(p)
{
    addParameter("MGDescent_samples", "25", "Number of MGDescent samples");
    addParameter("MGDescent_maxiterations", "10000", "Number of maximum iterations");
    addParameter("MGDescent_rate", "0.01", "The learning rate for gradient descent");
    addParameter("MGDescent_points", "1", "Number of random points for gradient descent");
}


bool MGDescent::checkifClose()
{
/*
    double m = 0.0;
    for (unsigned i = 0; i < r.size(); i++){
        m += (r[i] - rOld[i]) * (r[i] - rOld[i]);
    }
        m = fabs(sqrt(m));
         rOld=r;
    if (m<0.0001)
        return true;
        */
    return false;
}
bool MGDescent::terminated()
{
    int MGDescent_samples = params["MGDescent_samples"].toString().toInt();
    int MGDescent_maxiterations = params["MGDescent_maxiterations"].toString().toInt();
    if (MGDescent_maxiterations < MGDescent_samples)
        MGDescent_samples = MGDescent_maxiterations;
    return currentStep >= MGDescent_samples || checkifClose();
}

void MGDescent::step()
{

    ++currentStep;
    for ( unsigned i=0;i<r.size();i++ ){
        Data gradient;
        gradient.resize(r[i].size());
        learingRate.resize(r[i].size());
        Data lM = myProblem->getLeftMargin();
        Data rM = myProblem->getRightMargin();
        min = myProblem->funmin(r[i]);
        myProblem->granal(r[i], gradient);
        double meter=0.0;
        for(unsigned k=0;k<gradient.size();k++)
            meter += gradient[k] * gradient[k];

        learingRate[i] = gama * learingRate[i] + (1-gama)* meter ;
        printf("%d learingRate is %lf Quotient %lf Minimum% lf\n", currentStep, learingRate[i], rate/sqrt(learingRate[i] + 1e-7), min);

        for (unsigned j = 0; j < gradient.size(); j++)
        {
            r[i][j] -= rate * gradient[j]  / sqrt(learingRate[i] + 1e-7);
            if (r[i][j] > rM[j])
                r[i][j] = rM[j];
            if (r[i][j] < lM[j])
                r[i][j] = lM[j];
        }

        fprintf(pFile,"%d %lf\n",currentStep,min);

    }
}

void MGDescent::init()
{
    long d = myProblem->getDimension();
    cout<<"Dimensions = "<< d <<endl;
    points = params["MGDescent_points"].toString().toUInt();
    cout<<"Points = "<< points <<endl;
    rate = params["MGDescent_rate"].toString().toDouble();
    currentStep = 0;
    r.resize(points);
    rOld.resize(points);

    for ( unsigned i=0;i<points;i++ ){
        MGDescent::r[i] = myProblem->getRandomPoint();
     rOld[i]=r[i];
    }

    pFile =fopen("plot","w");
    gama=.4;
}

void MGDescent::done()
{
    fprintf(pFile,"%d %lf\n",currentStep,min);
    fclose(pFile);
    //min = myProblem->funmin(r);

    //cout<<"fgetBesty: "<< myProblem->getBesty() <<endl;
    //Data temp = myProblem->getLeftMargin();
    //Data temp2 = myProblem->getRightMargin();
    //cout<<"myProblem->fgetBestx: "<< myProblem->getBestx() <<endl;
    //cout<<"getLeftMargin: "<< myProblem->getLeftMargin() <<endl;
    //cout<<"getRightMargin: "<< myProblem->getRightMargin() <<endl;
    //cout<<"getleftmarginx: "<< myProblem->getleftmarginx() <<endl;
    //cout<<"getRandomPoint: "<< myProblem->getRandomPoint()<<endl;
    //cout<<"randomDouble(): "<< myProblem->randomDouble() <<endl;
}

MGDescent::~MGDescent()
{
}

extern "C" MGDESCENT_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MGDescent(p);
}

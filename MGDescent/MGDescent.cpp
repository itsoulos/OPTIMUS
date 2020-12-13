#include "MGDescent.h"
MGDescent::MGDescent(Problem *p)
    : Optimizer(p)
{
    addParameter("MGDescent_samples", "100", "Number of MGDescent samples");
    addParameter("MGDescent_maxiterations", "10000", "Number of maximum iterations");
    addParameter("MGDescent_rate", "0.01", "The learning rate for gradient descent");
    addParameter("MGDescent_points", "100", "Number of random points for gradient descent");
}


bool MGDescent::checkifClose()
{

    double m = 0.0;
    for ( unsigned i=0;i<points;i++ ){
        for (unsigned j = 0; j < r[i].size(); j++){
            m += (r[i][j] - rOld[i][j]) * (r[i][j] - rOld[i][j]);

        }
        m = fabs(sqrt(m));
        if (m<0.0001)
            return true;
        rOld[i]=r[i];
    }

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
    for ( unsigned i=0;i<points;i++ ){
        m[i] = myProblem->funmin(r[i]);
        myProblem->granal(r[i], gradient[i]);
        double meter=0.0;
        for(unsigned k=0;k<gradient[i].size();k++)
            meter += gradient[i][k] * gradient[i][k];
        learingRate[i] = gama * learingRate[i] + (1-gama)* meter ;
        //printf("%d learingRate is %lf Quotient %lf Minimum% lf\n", currentStep, learingRate[i], rate/sqrt(learingRate[i] + 1e-7), m[i]);
        for (unsigned j = 0; j < r[i].size(); j++)
        {
            r[i][j] -= rate * gradient[i][j] / sqrt(learingRate[i] + 1e-7);
            if (r[i][j] > rM[i][j])
                r[i][j] = rM[i][j];
            if (r[i][j] < lM[i][j])
                r[i][j] = lM[i][j];
        }
    }
    fprintf(pFile,"%d %lf\n",currentStep,min);
    min = *min_element(m.begin(),m.end());
    printf("%d] Minimum% lf\n", currentStep, min);

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
    m.resize(points);
    lM.resize(points);
    rM.resize(points);
    gradient.resize(points);
    for ( unsigned i=0;i<points;i++ ){
        r[i] = myProblem->getRandomPoint();
        rOld[i]=r[i];
        lM[i].resize(r[i].size());
        rM[i].resize(r[i].size());
        lM[i] = myProblem->getLeftMargin();
        rM[i] = myProblem->getRightMargin();
        gradient[i].resize(r[i].size());
        learingRate.resize(r[i].size());
    }

    pFile =fopen("plot","w");
    gama=.4;
}

void MGDescent::done()
{
    fprintf(pFile,"%d %lf\n",currentStep,min);
    fclose(pFile);

	int ipos=0;
	double dmin=m[0];
	for(int i=0;i<r.size();i++)
	{
		if(m[i]<dmin) {dmin=m[i];ipos=i;}
	}
    min = myProblem->funmin(r[ipos]);
}

MGDescent::~MGDescent()
{
}

extern "C" MGDESCENT_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MGDescent(p);
}

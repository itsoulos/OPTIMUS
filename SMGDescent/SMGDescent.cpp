#include "SMGDescent.h"
SMGDescent::SMGDescent(Problem *p)
    : Optimizer(p)
{
    addParameter("SMGDescent_samples", "100", "Number of SMGDescent samples");
    addParameter("SMGDescent_maxiterations", "10000", "Number of maximum iterations");
    addParameter("SMGDescent_rate", "0.01", "The learning rate for gradient descent");
    addParameter("SMGDescent_points", "100", "Number of random points for gradient descent");
}

bool SMGDescent::checkifClose()
{
    //critiria with learingRate
    double mm = 0.0;
    for (unsigned i = 0; i < r.size(); i++)
    {
        for (unsigned j = 0; j < r[i].size(); j++)
        {
            mm += (r[i][j] - rOld[i][j]) * (r[i][j] - rOld[i][j]) + (r[i][j] - rOld[i][j]) * (r[i][j] - rOld[i][j]);
        }
        mm = fabs(sqrt(mm));
        if (mm < 0.0005)
            return true;
    }
    return false;
}
bool SMGDescent::checkifClose2()
{
    //critiria with fluctuation
    double mo = 0.0;
    fluctuation = 0.0;
    for (unsigned i = 0; i < mins.size(); i++)
    {
        mo += mins[i];
    }
    mo /= mins.size() - 1;
    for (unsigned i = 0; i < mins.size(); i++)
    {
        fluctuation += pow((mins[i] - mo), 2);
    }
    fluctuation /= mins.size() - 1;
    if (fluctuation < 0.009) //0.009908
        return true;
    return false;
}
bool SMGDescent::terminated()
{
    int SMGDescent_samples = params["SMGDescent_samples"].toString().toInt();
    int SMGDescent_maxiterations = params["SMGDescent_maxiterations"].toString().toInt();
    if (SMGDescent_maxiterations < SMGDescent_samples)
        SMGDescent_samples = SMGDescent_maxiterations;
    return currentStep >= SMGDescent_samples;// || checkifClose2();
    //return currentStep >= SMGDescent_samples;
}
bool SMGDescent::criteria_f(vector<int> v, int p)
{
    for (auto x : v)
        if (x == p)
            return true;

    return false;
}
void SMGDescent::rejectedP()
{
    //int *pos;
    double d,max=0.0;
    int pos=-1;
    for (int i = 0; i < r.size() - 1; i++)
    {
        for (unsigned j = i + 1; j < r.size(); j++)
	{
        	d = 0.0;
            	for (unsigned t = 0; t < gradient[i].size(); t++)
                	d += pow((r[i][t] - r[j][t]), 2);
        	d = sqrt(fabs(d));

		if(d<0.5)
            	rejectedPoints.push_back(i);
	}
    }

    printf("-------------------------------------------\n");
    for (unsigned i = 0; i < rejectedPoints.size(); i++)
    {
        printf("%d] %d \n", i, rejectedPoints[i]);
    }

    printf("Maximum distance %lf at %d\n", max, pos);

}
void SMGDescent::step()
{
    ++currentStep;
    for (unsigned i = 0; i < r.size(); i++)
    {

        if (criteria_f(rejectedPoints, i))
            continue;
        rOld[i] = r[i];
        m[i] = myProblem->funmin(r[i]);
        myProblem->granal(r[i], gradient[i]);
        double meter = 0.0;
        //printf("%d\n", gradient[i].size());
        for (unsigned k = 0; k < gradient[i].size(); k++)
            meter += gradient[i][k] * gradient[i][k];
        learingRate[i] = gama * learingRate[i] + (1 - gama) * meter;
        //printf("%d learingRate is %lf Quotient %lf Minimum% lf\n", currentStep, learingRate[i], rate/sqrt(learingRate[i] + 1e-7), m[i]);

        for (unsigned j = 0; j < gradient[i].size(); j++)
        {
            r[i][j] -= rate * gradient[i][j] / sqrt(learingRate[i] + 1e-7);
            if (r[i][j] > rM[i])
                r[i][j] = rM[i];
            if (r[i][j] < lM[i])
                r[i][j] = lM[i];
        }
    }

    min = *min_element(m.begin(), m.end());
    mins.push_back(min);
    //fprintf(pFile,"%d %lf\n",currentStep,min);
    //printf("%d] Minimum% lf\n", currentStep, min);
    fprintf(pFile, "%d %lf %lf\n", currentStep, min, fluctuation);
    //printf("%d] Minimum %lf fluctuation %lf\n", currentStep, min, fluctuation);
}

void SMGDescent::init()
{
    long d = myProblem->getDimension();
    cout << "Dimensions = " << d << endl;
    points = params["SMGDescent_points"].toString().toUInt();
    cout << "Points = " << points << endl;
    rate = params["SMGDescent_rate"].toString().toDouble();
    currentStep = 0;
    r.resize(points);
    rOld.resize(r.size());
    m.resize(r.size());
    learingRate.resize(r.size());
    //rejectedPoints.resize(r.size());
    lM = myProblem->getLeftMargin();
    rM = myProblem->getRightMargin();
    gradient.resize(points);
    for (unsigned i = 0; i < r.size(); i++)
    {
        r[i] = myProblem->getRandomPoint();
        rOld[i] = r[i];
        gradient[i].resize(r[i].size());
    }
    pFile = fopen("plot", "w");
    gama = .4;
    rejectedP();
}

void SMGDescent::done()
{
    int ipos = 0;
    double dmin = m[0];
    for (unsigned i = 0; i < r.size(); i++)
    {
        if (m[i] < dmin)
        {
            dmin = m[i];
            ipos = i;
        }
    }
    min = myProblem->funmin(r[ipos]);
    //fprintf(pFile,"%d %lf\n",currentStep,min);
    fprintf(pFile, "%d %lf %lf\n", currentStep, min, fluctuation);
    fclose(pFile);
}

SMGDescent::~SMGDescent()
{
}

extern "C" SMGDESCENT_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new SMGDescent(p);
}

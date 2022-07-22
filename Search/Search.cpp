#include "Search.h"
Search::Search(Problem *p)
    : Optimizer(p)
{
    addParameter("Search_points", "100", "Number of random points for method");
    addParameter("Search_maxiterations", "10000", "Number of maximum iterations");
    addParameter("Search_rate", "0.01", "The learning rate for method");
    addParameter("Search_method", "4", "search method");
}

bool Search::checkifClose()
{
    // critiria with learingRate
    double mm = 0.0;
    for (unsigned i = 0; i < r.size() - 1; i++)
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
bool Search::checkifClose2()
{

    // critiria with fluctuation
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
    if (fluctuation < 0.009) // 0.009908
        return true;
    return false;
}
bool Search::terminated()
{

    if (Search_maxiterations < Search_points)
        Search_points = Search_maxiterations;
    return currentStep >= Search_points || checkifClose();
    // return currentStep >= Search_points;
}

void Search::gd()
{
    for (unsigned i = 0; i < points; i++)
    {
        m[i] = myProblem->funmin(r[i]);
        rOld[i] = r[i];
        myProblem->granal(r[i], gradient[i]);
        double meter = 0.0;
        for (unsigned k = 0; k < gradient[i].size(); k++)
            meter += gradient[i][k] * gradient[i][k];
        learingRate[i] = gama * learingRate[i] + (1 - gama) * meter;
        double d = 0.0;
        for (unsigned j = 0; j < gradient[i].size(); j++)
        {
            r[i][j] -= rate * gradient[i][j] / sqrt(learingRate[i] + 1e-7);
            if (r[i][j] > rM[j])
                r[i][j] = rM[j];
            if (r[i][j] < lM[j])
                r[i][j] = lM[j];
        }

    }
}
void Search::sgd()
{
    for (unsigned i = 0; i < points; i++)
    {
        m[i] = myProblem->funmin(r[i]);
        rOld[i] = r[i];
        myProblem->granal(r[i], gradient[i]);
        double meter = 0.0;
        for (unsigned k = 0; k < gradient[i].size(); k++)
            meter += gradient[i][k] * gradient[i][k];
        //learingRate[i] = gama * learingRate[i] + (1 - gama) * meter;
        learingRate[i]=0.00001;
        double d = 0.0;
        for (unsigned j = 0; j < dimension; j++)
        {
            r[i][j] -= rate * gradient[i][j] / sqrt(learingRate[i] + 1e-7);
            if (r[i][j] > rM[j])
                r[i][j] = rM[j];
            if (r[i][j] < lM[j])
                r[i][j] = lM[j];
        }
    }
}
void Search::mbgd()
{
    for (unsigned i = 0; i < points; i++)
    {
        m[i] = myProblem->funmin(r[i]);
        rOld[i] = r[i];
        myProblem->granal(r[i], gradient[i]);
        double meter = 0.0;
        for (unsigned k = 0; k < gradient[i].size(); k++)
            meter += gradient[i][k] * gradient[i][k];
        learingRate[i] = gama * learingRate[i] + (1 - gama) * meter;
    }
    //....
}
void Search::adam()
{
    double f = 0.0;
    for (int i = 0; i < points; i++)
    {
        rOld[i] = r[i];
        myProblem->granal(r[i], gradient[i]);
        double a = lRate * sqrt(1.0 - pow(b2, i + 1)) / (1.0 - pow(b1, i + 1));
        for (unsigned j = 0; j < dimension; j++)
        {
            M_0[i][j] = b1 * M_0[i][j] + (1 - b1) * gradient[i][j];
            U_0[i][j] = b2 * U_0[i][j] + (1 - b2) * gradient[i][j] * gradient[i][j];
            r[i][j] -= a * M_0[i][j] / (sqrt(U_0[i][j]) + 1e-1);
            if (r[i][j] > rM[j])
                r[i][j] = rM[j];
            if (r[i][j] < lM[j])
                r[i][j] = lM[j];
        }
        m[i] = myProblem->funmin(r[i]);
    }
}
void Search::step()
{
    ++currentStep;
    switch (Search_method)
    {
    case 1:
        gd(); // gradient descent
        break;
    case 2:
        sgd(); // s gradient descent
        break;
    case 3:
        mbgd(); // mini batch gradient descent
        break;
    case 4:
        adam(); // adam
        break;
    }
    min = *min_element(m.begin(), m.end());
    mins.push_back(min);
    fprintf(pFile, "%d %lf %lf\n", currentStep, min, fluctuation);
}

void Search::init()
{
    dimension = myProblem->getDimension();
    points = params["Search_points"].toString().toUInt();
    rate = params["Search_rate"].toString().toDouble();
    Search_points = params["Search_points"].toString().toInt();
    Search_maxiterations = params["Search_maxiterations"].toString().toInt();
    Search_method = params["Search_method"].toString().toInt();
    lM = myProblem->getLeftMargin();
    rM = myProblem->getRightMargin();
    currentStep = 0;
    r.resize(points);
    rOld.resize(points);
    m.resize(points);
    learingRate.resize(points);
    gradient.resize(points);
    M_0.resize(points);
    U_0.resize(points);
    for (unsigned i = 0; i < r.size(); i++)
    {
        r[i] = myProblem->getRandomPoint();
        rOld[i] = r[i];
        gradient[i].resize(dimension, 0.0);
        M_0[i].resize(dimension, 0.0);
        U_0[i].resize(dimension, 0.0);
    }
    pFile = fopen("plot", "w");
    gama = .4;
    dmin = 0.0;
    imin = 0;
    b1 = 0.9;
    b2 = 0.999;
    lRate = 0.25;
}

void Search::done()
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
    // fprintf(pFile,"%d %lf\n",currentStep,min);
    // fprintf(pFile, "%d %lf %lf\n", currentStep, min, fluctuation);
    fclose(pFile);
    // printf("\n-------------------------------------------------------------------\n");
}

Search::~Search()
{
}

extern "C" SEARCH_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Search(p);
}

#include "simplexmethod.h"

SimplexMethod::SimplexMethod(Problem *p)
{
    myProblem = p;
    alpha = 1.0;
    gamma = 2.0;
    beta = 0.5;
}

Data SimplexMethod::getCentroid()
{
    Data x;
    int n = myProblem->getDimension();
    x.resize(n);
    for (int i = 0; i <= n - 1; i++)
    {
        x[i] = 0.0;
        for (int j = 0; j < n; j++)
            x[i] += xpoint[i][j];
        x[i] /= n;
    }
    return x;
}

Data SimplexMethod::getReflected(Data &centroid)
{
    Data x;
    int n = myProblem->getDimension();
    x.resize(n);
    for (int i = 0; i < n; i++)
    {
        x[i] = centroid[i] + alpha * (centroid[i] - xpoint[n][i]);
    }
    return x;
}

void SimplexMethod::sortPoints()
{
    int n = myProblem->getDimension();
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (ypoint[j] > ypoint[j + 1])
            {
                double t = ypoint[j];
                ypoint[j] = ypoint[j + 1];
                ypoint[j + 1] = t;
                Data xt;
                xt = xpoint[j];
                xpoint[j] = xpoint[j + 1];
                xpoint[j + 1] = xt;
            }
        }
    }
}

double SimplexMethod::getMeanf()
{
    double mf = 0.0;
    int n = myProblem->getDimension();
    for (int i = 0; i <= n; i++)
        mf += ypoint[i];
    return mf / (n + 1);
}

double SimplexMethod::getVariancef()
{
    double sigma = 0.0;
    double mf = getMeanf();
    printf("Simplex mean = %lf \n",mf);
    int n = myProblem->getDimension();
    for (int i = 0; i <= n; i++)
        sigma += pow(ypoint[i] - mf, 2.0);
    return sigma / n;
}

void SimplexMethod::insertionStep()
{
    // insert fr in the correct place
    int pos = -1;
    int n = myProblem->getDimension();
    for (int i = 0; i < n; i++)
    {
        if (fr >= ypoint[i] && fr <= ypoint[i + 1])
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
        pos = n;
    xpoint[pos] = xr;
    ypoint[pos] = fr;
}

void SimplexMethod::stretchingStep()
{
    Data xe;
    int n = myProblem->getDimension();
    xe.resize(n);
    for (int i = 0; i < n; i++)
        xe[i] = xc[i] + gamma * (xr[i] - xc[i]);
    double fe = myProblem->funmin(xe);
    if (fe < fr)
    {
        xpoint[n] = xe;
        ypoint[n] = fe;
    }
    else
    {
        xpoint[n] = xr;
        ypoint[n] = fr;
    }
}

void SimplexMethod::contractionStep()
{
    Data xk;
    int n = myProblem->getDimension();
    xk.resize(n);
    if (fr < ypoint[n])
    {
        for (int i = 0; i < n; i++)
            xk[i] = xc[i] + beta * (xr[i] - xc[i]);
    }
    else
    {
        for (int i = 0; i < n; i++)
            xk[i] = xc[i] + beta * (xpoint[n][i] - xc[i]);
    }
    double fk = myProblem->funmin(xk);
    double dmin = fr < ypoint[n] ? fr : ypoint[n];
    if (fk < dmin)
    {
        xpoint[n] = xk;
        ypoint[n] = fk;
    }
    else
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                xpoint[i][j] = 0.5 * (xpoint[0][j] + xpoint[i][j]);
            }
            ypoint[i] = myProblem->funmin(xpoint[i]);
        }
    }
}

void SimplexMethod::setAlpha(double a)
{
    alpha = a > 0 ? a : 2.0;
}

double SimplexMethod::getAlpha() const
{
    return alpha;
}

void SimplexMethod::setBeta(double b)
{
    beta = (b > 0) ? b : 0.5;
}

double SimplexMethod::getBeta() const
{
    return beta;
}

void SimplexMethod::setGamma(double g)
{
    gamma = (g > 0) ? g : 2.0;
}

double SimplexMethod::getGamma() const
{
    return gamma;
}

void SimplexMethod::runAlgorithm()
{
    // sample
    const double eps = 1e-6;
    int n = myProblem->getDimension();
    xpoint.resize(n + 1);
    ypoint.resize(n + 1);
    for (int i = 0; i <= n; i++)
    {

        xpoint[i] = myProblem->getRandomPoint();

        ypoint[i] = myProblem->funmin(xpoint[i]);
    }
    int iter = 0;
    while (true)
    {
        sortPoints();
        double s = getVariancef();
        if (s < eps)
            break;
        xc = getCentroid();
        fc = myProblem->funmin(xc);
        xr = getReflected(xc);
        fr = myProblem->funmin(xr);
        if (fr >= ypoint[0] && fr <= ypoint[n - 1])
            insertionStep();
        else if (fr <= ypoint[0])
            stretchingStep();
        else
            contractionStep();
        iter++;
        printf("ITER=%4d BEST VALUE=%20.10lg VARIANCE=%20.10lg\n",
               iter, ypoint[0], s);
    }
}

Data    SimplexMethod::getBest()
{
    return xpoint[0];

}

SimplexMethod::~SimplexMethod()
{
}

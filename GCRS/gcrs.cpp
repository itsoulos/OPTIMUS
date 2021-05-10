#include "gcrs.h"

Gcrs::Gcrs(Problem *p)
    : Optimizer(p)
{
    addParameter("gcrs_samples", "25", "Number of gcrs samples");
    addParameter("gcrs_maxiterations", "200", "Number of maximum iterations");
}

bool Gcrs::terminated()
{

    int gcrs_maxiterations = params["gcrs_maxiterations"].toString().toInt();
	double my = fabs(1.0 + fabs(fmin));
    x1 = x1 + my;
    x2 = x2 + my * my;
    variance = x2 / (iteration + 1) - ( x1 / (iteration + 1) ) * ( x1 / (iteration + 1) );
    variance=fabs(variance);
    if( fmin < oldBesty)
    {
        oldBesty = fmin; 
        stopat = variance / 2.0;
    }
    if( stopat < 1e-8 && iteration >=10 ) return true;
    printf("GCRS. Iteration: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n", iteration, fmin, variance, stopat);
    return (( iteration >=gcrs_maxiterations ) || ( variance <= stopat && iteration >= 20 ));
    //return (iteration >= gcrs_maxiterations || stopIt);
}

void Gcrs::step()
{
    int gcrs_samples = params["gcrs_samples"].toString().toInt();
    ++iteration;
    stopIt = false;
    if (flagF == true)
    {
        fmax = -1e+100;
        fmin = 1e+100;
        for (int i = 0; i < M; i++)
        {
            y = sample[i].second; //min
            if (y < fmin)
            {
                posmin = i;
                fmin = y;
                xmin = sample[i].first; //data min
            }
            if (y > fmax)
            {
                posmax = i;
                fmax = y;
                xmax = sample[i].first; //data max
            }
        }
        /*printf("iters=%d fmin=%.10lg diff=%.10lg\n",iteration,fmin,fabs(fmax-fmin));
        if (fabs(fmax - fmin) < 1e-6)
        {
            stopIt = true;
            sample2.clear();
        }
        */
    }
    do
    {
        sample2.clear();

        for (int i = 0; i < dimension; i++)
            center[i] = 0.0;
        for (int i = 0; i < dimension + 1; i++)
            index[i] = -1;
        for (int i = 0; i < dimension + 1; i++)
        {
            int r, flag;
            do
            {
                r = rand() % M;
                flag = 0;
                for (int j = 0; j < i; j++)
                {
                    if (index[j] == r)
                    {
                        flag = 1;
                        break;
                    }
                }
            } while (flag);
            index[i] = r;
            x = sample[r].first;
            y = sample[r].second;
            sample2[i] = pair<Data, double>(x, y);
            //sample2.insert({{i}, {x, y}});
            //sample.insert( {{i},{pair<Data, double>(x, y)}});
            if (i != dimension)
                for (int j = 0; j < dimension; j++)
                    center[j] = center[j] + 1.0 / dimension * x[j];
        }
        x = sample2[dimension].first;
        for (int i = 0; i < dimension; i++)
            xk[i] = 2.0 * center[i] - x[i];
    } while (!myProblem->isPointIn(xk));
    fk = myProblem->funmin(xk);
    success += (fk <= fmax);
    if (fk <= fmax)
    {
        sample[posmax] = pair<Data, double>(xk, fk);
        flagF = true;
    }

    if (fk > fmax)
    {
        if (success * 1.0 / iteration > 0.5)
        {
            flagF = false;
        }
        else
        {
            for (int i = 0; i < dimension; i++)
                xk[i] = 0.5 * center[i] + 0.5 * x[i];
            if (!myProblem->isPointIn(xk))
                flagF = false;
            fk = myProblem->funmin(xk);
            success += (fk <= fmax);

            if (fk < fmax)
            {
                sample[posmax] = pair<Data, double>(xk, fk);
                flagF = true;
            }
            else
                flagF = false;
        }
    }

}

void Gcrs::init()
{
    int gcrs_samples = params["gcrs_samples"].toString().toInt();
    iteration = 0;
    flagF = true;
    dimension = myProblem->getDimension();
    M = gcrs_samples * dimension;
    //M = gcrs_samples;
    xmin.resize(dimension);
    xmax.resize(dimension);
    center.resize(dimension);
    index.resize(dimension + 1);
    xk.resize(dimension);
    success = 0;
    posmax = 0;
    posmin = 0;
    x1 = 0.0;
    x2 = 0.0;
    variance = 0.0;
    stopat = 0.0;
    oldBesty = 1e+100;
    for (int i = 0; i < M; i++)
    {
        Data point = myProblem->getRandomPoint();
        y = myProblem->funmin(point);
        sample[i] = pair<Data, double>(point, y);
        //sample.insert( {{i},{pair<Data, double>(point, y)}});
    }
}

void Gcrs::done()
{
    double f = myProblem->funmin(xmin);
    y = localSearch(xmin);
    {
            if(y<fmin)
                fmin=y;
    }
}

void Gcrs::Solve()
{
    Optimizer::Solve();
}

Gcrs::~Gcrs()
{
   sample.clear();
   sample2.clear();
}

extern "C" GCRS_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Gcrs(p);
}

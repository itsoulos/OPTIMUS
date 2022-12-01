#include "islandde.h"

islandDE::islandDE(Problem *p)
    : Optimizer(p)
{
    prin = std::chrono::system_clock::now();
    addParameter("islandde_samples", "100", "Number of multistart samples");
    addParameter("islandde_maxiterations", "100", "Number of maximum iterations");
    addParameter("islandde_islands", "5", "Number of maximum islands");
    addParameter("crossoverProbability", "0.9", "Crossover Probability");
    addParameter("differentialWeight", "0.8", "Differential Weight");
}

vector<vector<pair<double, Data>>> islandDE::fragmentationToIslands(vector<Data> Samples, vector<double> ySamples, int n)
{
    vector<vector<pair<double, Data>>> allIslands;
    vector<pair<double, Data>> pointsOfIsland;
    vector<double> fit;
    int div = Samples.size() / n, j = 0, t = 0;
    Data tmp_y;
    // printf("div = %d n = %d \n", div, n );
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < div; j++)
        {
            Data d = Samples.at(j + t);
            double y = ySamples.at(j + t);
            pair<double, Data> p;
            p.first = y;
            p.second = d;
            pointsOfIsland.push_back(p);
            fit.push_back(y);
            tmp_y.push_back(1e+100);
        }
        fitness_array.push_back(fit);
        allIslands.push_back(pointsOfIsland);
        pointsOfIsland.clear();
        t += div;
        // printf("j = %d , t = %d\n", j, t );
        sum.push_back(0);
        yis.push_back(tmp_y);
    }

    int pil = Samples.size() % n;
    if (pil != 0)
    {
        for (int k = Samples.size() - pil; k < Samples.size(); k++)
        {
            // printf("k = %d Samples = %d \n", k, Samples.size() );
            Data d = Samples.at(k);
            double y = ySamples.at(k);
            pair<double, Data> p;
            p.first = y;
            p.second = d;
            pointsOfIsland.push_back(p);
            fit.push_back(y);
            tmp_y.push_back(1e+100);
        }
        fitness_array.push_back(fit);
        // allIslands.at(n-1).push_back(pointsOfIsland);
        allIslands.push_back(pointsOfIsland);
        sum.push_back(0);
        yis.push_back(tmp_y);
    }

    newSum.resize(sum.size());
    return allIslands;
}
void islandDE::singleDE(vector<pair<double, Data>> samples, int islandName, Data &xmin, double &ymin)
{
    xmin.resize(dimension);
    int i, j, R, k;
    double a, b, c,newMin;
    Data z, r, newX;
    z.resize(dimension);
    r.resize(dimension);
    newX.resize(dimension);

    //vector<pair<double, Data>> s;// = samples;
#pragma omp parallel for private(i,j,k,newMin,a,b,c) \
shared(yis,ymin,xmin,islandName,fitness_array,samples,z,r,newX ) num_threads(threads)
    for (i = 0; i < samples.size(); i++)
    {
        a = i;
        b = i;
        c = i;
        while (a == i || b == i || c == i || a == b || a == c || b == c)
        {
            a = rand() % samples.size();
            b = rand() % samples.size();
            c = rand() % samples.size();
            if (a < 0)
                a = i;
            if (b < 0)
                b = i;
            if (c < 0)
                c = i;
        }

        for (j = 0; j < dimension; j++)
        {
//#pragma omp critical
            z.at(j) = samples.at(a).second.at(j) + differentialWeight * (samples.at(b).second.at(j) - samples.at(c).second.at(j));
        }

//#pragma omp critical
        R = rand() % samples.size();

        for (double &var : r)
        {
            var = randMToN(0, 1);
        }

        for (k = 0; k < dimension; k++)
        {

            if (r.at(k) < crossoverProbability || k == R && (z.at(k) >= lmargin[k] && z.at(k) <= rmargin[k]))
            {
//#pragma omp critical
                newX[k] = z[k];
            }
            else
            {
//#pragma omp critical
                newX[k] = samples.at(i).second.at(k);
            }
        }

        newMin = myProblem->funmin(newX);
        //newMin = localSearch(newX);
#pragma omp critical
{
        if (newMin < yis.at(islandName).at(i))
        {
            samples.at(i).second = newX;
            xmin = newX;
            samples.at(i).first = newMin;
            ymin = newMin;
            yis.at(islandName).at(i) = newMin;
            fitness_array.at(islandName).at(i) = newMin;

}

    }
    }

    for (int j = 0; j < samples.size(); j++)
        newSum.at(islandName) += fitness_array.at(islandName).at(j);
    newSum.at(islandName) /= (double)samples.size();
}
bool islandDE::checkIsland(int islandName, int newSum)
{
    double dd = fabs(newSum - sum.at(islandName));
    // printf("%4d] Generation  change: %10.6lf \n", iteration, dd);
    sum.at(islandName) = newSum;
    if (dd < 1e-8)
        n++;
    else
        n = 0;
    if (n > 15)
        return true;
    return iteration >= maxIt;
}
bool islandDE::terminated()
{
    int c = 0;
    for (int i = 0; i < ni; i++)
        if (this->checkIsland(i, newSum.at(i)))
            c++;
     c=0;
    return iteration >= maxIt || c == ni;
}

void islandDE::step()
{
    Data xx;
    double yy;
    int i,ii;

//#pragma omp parallel for private(i, xx, yy) num_threads(threads) \
shared(newSum, allIsland, ii)
    for (i = 0; i < allIsland.size(); i++)
    {
        ii=i;

        if (this->checkIsland(ii, newSum.at(ii)))
            this->singleDE(allIsland.at(ii), ii, xx, yy);
#pragma omp critical
        {
        if (yy < yOld)
        {
            mbestx = xx;
            mbesty = yy;
            yOld = yy;
        }
        }
    }
    ++iteration;
}

void islandDE::init()
{

    iteration = 0;
    dimension = myProblem->getDimension();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    mbestx.resize(dimension);
    allSamples = params["islandde_samples"].toString().toInt();
    maxIt = params["islandde_maxiterations"].toString().toInt();
    ni = params["islandde_islands"].toString().toInt();
    crossoverProbability = params["crossoverProbability"].toString().toDouble();
    differentialWeight = params["differentialWeight"].toString().toDouble();
    sampleFromProblem(allSamples, xsample, ysample);
    // uniformDistribution(allSamples, xsample, ysample);
    allIsland.resize(ni);
    allIsland = fragmentationToIslands(xsample, ysample, ni);
    // this->viewAllSamples(allIsland);
    n = 0;
    yOld = 1e+100;
}

void islandDE::done()
{
    double y = localSearch(mbestx);
    meta = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(meta - prin);
    auto ms = milliseconds.count();
    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;

    //    for (int i=0;i<allIsland.size();i++)
    //        printf("size of %d = %d\n", i, allIsland.at(i).size() );
}

void islandDE::Solve()
{
    Optimizer::Solve();
}

islandDE::~islandDE()
{
}

void islandDE::viewAllSamples(vector<vector<pair<double, Data>>> all)
{
    for (int i = 0; i < all.size(); i++)
    {
        printf("\n-------------------\n");
        for (int j = 0; j < all.at(i).size(); j++)
        {
            printf("[");
            for (int k = 0; k < all.at(i).at(j).second.size(); k++)
            {
                printf(" %f ", all.at(i).at(j).second.at(k));
            }
            printf("] ");
        }
    }
}

void islandDE::uniformDistribution(int &N, Matrix &xsample, Data &ysample)
{
    QRandomGenerator64 rd;

    std::generate_canonical<qreal, std::numeric_limits<qreal>::digits>(rd);
    int i;
    Matrix xpoint;
    Data ypoint;
    int n = myProblem->getDimension();
    // std::random_device rd;
    std::mt19937 gen(rd);
    Data lmm = myProblem->getLeftMargin();
    Data rmm = myProblem->getRightMargin();
    double lm = lmm.at(0);
    double rm = rmm.at(0);
    Data sample;
    // QRandomGenerator::bounded()->(lm,rm);
    // QRandomGenerator::global()->bounded(lm,rm);
    std::uniform_real_distribution<> dis(lm, rm);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double randomDouble = dis(gen);
            sample.push_back(randomDouble);
        }
        xpoint.push_back(sample);
        ypoint.push_back(myProblem->funmin(xpoint.at(i)));
    }
    xsample = xpoint;
    ysample = ypoint;
}
double islandDE::randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}
extern "C" ISLANDDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new islandDE(p);
}

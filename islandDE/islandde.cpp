#include "islandde.h"

islandDE::islandDE(Problem *p)
    : Optimizer(p)
{
    prin = std::chrono::system_clock::now();
    addParameter("islandde_samples", "1000", "Number of multistart samples");
    addParameter("islandde_maxiterations", "100", "Number of maximum iterations");
    addParameter("islandde_islands", "5", "Number of islands");
    addParameter("crossoverProbability", "0.9", "Crossover Probability");
    addParameter("differentialWeight", "0.8", "Differential Weight");
    addParameter("stopping_rule", "1", "stopping_rule[1,2,3]");
    addParameter("tournament", "0", "tournament[0,1]");
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
            // fit.push_back(y);
            // fitness_array.at(j)=y;
        }

        allIslands.push_back(pointsOfIsland);
        pointsOfIsland.clear();
        t += div;
        // printf("j = %d , t = %d\n", j, t );
        //sum.push_back(0);
        bestMin.at(i)=1e+100;

    }
    // printf("allIslandsSize=%d\n", allIslands.size() );
    // this->viewAllSamples(allIslands);
    /*
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
            }
            fitness_array.push_back(fit);
            // allIslands.at(n-1).push_back(pointsOfIsland);
            allIslands.push_back(pointsOfIsland);
            sum.push_back(0);
        }
    */
    fitness_array.resize(ni);
    for (int i = 0; i < allIslands.size(); i++)
    {
        fitness_array.at(i).resize(allIslands.at(i).size());
    }
    for (int i = 0; i < allIslands.size(); i++)
    {
        for (int j = 0; j < allIslands.at(i).size(); j++)
        {
            fitness_array.at(i).at(j) = allIslands.at(i).at(j).first;
            // printf(" %f ", allIslands.at(i).at(j).first);
        }
    }

    for (int i = 0; i < ni; i++)
    {
        sum.at(i) = accumulate(fitness_array.at(i).begin(), fitness_array.at(i).end(), 0);
    }
    //sum = accumulate(fitness_array.begin(), fitness_array.end(), 0);
    for (int i = 0; i < ni; i++)
    {
        sum.at(i) = accumulate(fitness_array.at(i).begin(), fitness_array.at(i).end(), 0);
    }
    return allIslands;
}
int islandDE::tournament(int N, int NN)
{
    int bestPos = -1;
    double bestValue = 1e+100;
    int k = allIsland.at(NN).size();
    //QRandomGenerator gen =  QRandomGenerator();
    //int pos = gen.bounded(k);
/*
    for (int i = 0; i < N; i++)
    {
        int pos = rand() % allIsland.at(NN).size();
        if (allIsland.at(NN).at(pos).first < bestValue)
        {
             bestPos = pos;
             bestValue = allIsland.at(NN).at(pos).first;
        }
    }
    */
    for (int i = 0; i < N; i++)
    {
        int pos = rand() % allIsland.at(NN).size();
        if (fitness_array.at(NN).at(pos) < bestValue)
        {
             bestPos = pos;
             bestValue = fitness_array.at(NN).at(pos);
        }
    }
    return bestPos;
}

void islandDE::singleDE(vector<pair<double, Data>> samples, int islandName, Data &xmin, double &ymin)
{
    xmin.resize(dimension);
    int i, j, R, k, a, b, c;
    Data z, r, newX, newMin;
    z.resize(dimension);
    r.resize(dimension);
    newX.resize(dimension);
    newMin.resize(samples.size());
    Data tmp;
    tmp.resize(dimension);
    Data tmp2;
    tmp2.resize(dimension);

    QRandomGenerator gen1 = QRandomGenerator();                 //::securelySeeded()
    QRandomGenerator gen2 = QRandomGenerator();                 //::securelySeeded()
    //QRandomGenerator gen3 = QRandomGenerator();

    // #pragma omp parallel for default(shared) private(i,a,b,c,gen1, gen2) num_threads(threads)
    for (i = 0; i < samples.size(); i++)
    {

        a = i;
        b = i;
        c = i;
        int n = samples.size();
        while (a == i || b == i || c == i || a == b || a == c || b == c)
        {
            // a = rand() % samples.size();
            // b = rand() % samples.size();
            // c = rand() % samples.size();

            if (tour==0)
            {
                a = gen1.bounded(n);
                b = gen1.bounded(n);
                c = gen1.bounded(n);
            }else
            {
                a = tournament(4, islandName);
                b = tournament(4, islandName);
                c = tournament(4, islandName);
            }
            if (a < 0)
                a = i;
            if (b < 0)
                b = i;
            if (c < 0)
                c = i;        }

        for (j = 0; j < dimension; j++)
        {
            z.at(j) = samples.at(a).second.at(j) + differentialWeight * (samples.at(b).second.at(j) - samples.at(c).second.at(j));
            // tmp.at(j)=z.at(j);
        }

        R = gen1.bounded(n);
/*
        if (true)
        {

            double f;
            if (bestMax.at(islandName) / bestMin.at(islandName) < 1)
                f = 1.0 - (bestMax.at(islandName) / bestMin.at(islandName));
            else
                f = 1.0 - (bestMin.at(islandName) / bestMax.at(islandName));
            differentialWeight = f;
        }
       else
        {
            //differentialWeight = -0.5 + 2.0 * gen3.bounded(1.0);
            // differentialWeight = -0.5 + 2.0 * drand48();
        }*/

        // #pragma omp parallel for default(shared) private(j)
        for (j = 0; j < dimension; j++)
        {

            r.at(j) = gen2.bounded(1.0);
        }

        for (k = 0; k < dimension; k++)
        {

            if ((r.at(k) < crossoverProbability || k == R) && (z.at(k) >= lmargin[k] && z.at(k) <= rmargin[k]))
            {
                newX.at(k) = tmp.at(k);
                // tmp2.at(k) = newX.at(k);
            }
            else
            {
                newX.at(k) = samples.at(i).second.at(k);
                // tmp2.at(k) = newX.at(k);
            }
        }

        newMin.at(i) = myProblem->funmin(newX);

        if (newMin.at(i) < fitness_array.at(islandName).at(i))
        {
            samples.at(i).first = newMin.at(i);
            samples.at(i).second = newX;
            xmin = newX;
            ymin = newMin.at(i);
            fitness_array.at(islandName).at(i) = newMin.at(i);
            bestMin.at(islandName) = newMin.at(i);
            bestMax.at(islandName) = *max_element(fitness_array.at(islandName).begin(), fitness_array.at(islandName).end());
        }
    }
    for (int j = 0; j < ni; j++)
    {
        newSum.at(j) = accumulate(fitness_array.at(j).begin(), fitness_array.at(j).end(), 0);
    }
    for (int j = 0; j < ni; j++)
        newMO.at(j) = (double)newSum.at(j)/ni;

}
bool islandDE::checkIsland(int islandName, int newValue)
{
    if (stoppingRule==1)// μεταβολή μέσοου όρου για n
    {
        newMO.at(islandName)=(double)newValue/ni;
        double dd = fabs(newMO.at(islandName) - MO.at(islandName));
        printf("%4d] newMO.at(%d) : %f MO.at(%d) : %f different  : %lf\n", iteration,islandName, newMO.at(islandName),islandName, MO.at(islandName),islandName,dd);
        MO.at(islandName) = newMO.at(islandName);
        if (dd < 1e-6)
            n++;
        else
            n = 0;
        if (n > 15)
            return true;
    }else if (stoppingRule==2)// μεταβολή ποσοτήτων για n
    {
        newSum.at(islandName)= newValue;
        double dd = fabs(newSum.at(islandName) - sum.at(islandName));
        printf("%4d] neWsum.at(%d) : %f Sum.at(%d) : %f different  : %lf\n", iteration,islandName, newSum.at(islandName),islandName, sum.at(islandName),islandName,dd);
        sum.at(islandName) = newValue;
        if (dd < 1e-6)
            n++;
        else
            n = 0;
        if (n > 15)
            return true;
    }else if (stoppingRule==3)// ali άχρηστο
    {
        double dd = fabs(bestMin.at(islandName) - bestMax.at(islandName));
        printf("%4d] bestMax.at(%d) : %10.6lf bestMin.at(%d) : %10.6lf different  : %10.6lf\n", iteration,islandName, bestMax.at(islandName),islandName, bestMin.at(islandName),islandName,dd);
        if (dd < 1e-2)
            return true;
    }
    return false;
}
bool islandDE::terminated()
{
    int c = 0;
    for (int i = 0; i < ni; i++)
    {
        if (this->checkIsland(i, newSum.at(i)))
            c++;
    }
    if (stoppingRule == 0)
        c = 0;
    return iteration >= maxIt || c >= ni;
}

void islandDE::step()
{
    Data xx;
    double yy;
    int i;

#pragma omp parallel for private(i, xx, yy) num_threads(threads) default(shared)
    for (i = 0; i < allIsland.size(); i++)
    {
        this->singleDE(allIsland.at(i), i, xx, yy);
        if (yy < yOld)
        {
            mbestx = xx;
            mbesty = yy;
            yOld = yy;
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
    bestMin.resize(ni);
    bestMax.resize(ni);
    sum.resize(ni);
    newSum.resize(ni);
    newMO.resize(ni);
    MO.resize(ni);
    allIsland = fragmentationToIslands(xsample, ysample, ni);
    // this->viewAllSamples(allIsland);
    n = 0;
    yOld = 1e+100;

    stoppingRule = params["stopping_rule"].toString().toInt();
    tour = params["tournament"].toString().toInt();
    // printf("stopping_rule=%d\n", stoppingRule);
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
    // srand(1);
    return M + (rand() / (RAND_MAX / (N - M)));
}
extern "C" ISLANDDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new islandDE(p);
}

#include "mincenter.h"

MinCenter::MinCenter(Problem *p)
    : Optimizer(p)
{
    addParameter("mincenter_samples", "2000", "Number of samples taken");
    addParameter("mincenter_centers", "500", "Number of centers");
    addParameter("mincenter_iterations", "200", "Number of iterations");
}

bool MinCenter::terminated()
{
    return currentIteration >= iterations; 
}

void MinCenter::step()
{
    ++currentIteration;
    for (int i = 0; i < samples; i++)
    {
        std::vector<double> v = myProblem->getRandomPoint();
        Point point(i, v);
        allSamples.push_back(point);
    }
    kmeans->run(allSamples);
}

void MinCenter::init()
{
    currentIteration = 0;
    iterations = params["mincenter_iterations"].toString().toInt();
    centers = params["mincenter_centers"].toString().toInt();
    samples = params["mincenter_samples"].toString().toInt();
    minK = params["mincenter_minimum_centers"].toString().toInt();
    int it = 100;
    kmeans = new KMeans(centers, it, minK);
    allmeans.resize(centers);
}

vector<Point> MinCenter::meansWithFewPoints()
{
    std::vector<int> s = kmeans->getSizes();
    for (unsigned i = 0; i < s.size(); i++)
    {
        if (s[i] < rate)
        {
            kmeans->deleteMean(i);
        }
    }
    kmeans->run(allSamples);
    return kmeans->getMeans();
}

vector<Point> MinCenter::nearMeans(vector<Point> m)
{

    vector<Point> tmp = m;
    double min = Point::distance(m.at(0), m.at(1));
    unsigned i, j, e = 0;
    double d = 0.0;
    min = 1e+100;
    for (i = 0; i < m.size(); i++)
    {
        for (j = i + 1; j < m.size(); j++)
        {
            double dis = Point::distance(m.at(i), m.at(j));
            if (min > dis)
            {
                min = dis;
            }
        }
    }
    d = min;
    d *= 1.2; // 20%
    for (i = 0; i < m.size(); i++)
        for (j = i + 1; j < m.size(); j++)
        {
            double distance = Point::distance(m.at(i), m.at(j));
            if (distance <= d)
            {
                tmp.erase(tmp.begin() + i);
                break;
            }
        }
    return tmp;
}

void MinCenter::done()
{
    rate = (iterations * samples) / centers;
    rate *= .95; //95%
    Data bestx;
    double mbesty = 1e+100;

    vector<Point> tmp = meansWithFewPoints();
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("\nInit samples => %d \n", samples);
    printf("Init centers => %d \n", centers);
    printf("CENTERS (means with few points) ==> %ld\n", centers - allmeans.size());
    tmp = nearMeans(allmeans);
    int k = allmeans.size();
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("CENTERS (nearby means) ==> %ld\n", k - allmeans.size());
    printf("TOTAL CENTERS ==> %ld\n", allmeans.size());

#pragma omp parallel for num_threads(threads)
    for (unsigned int i = 0; i < allmeans.size(); i++)
    {
        Point p = allmeans[i];
        Tolmin mTolmin(myProblem);
        Data trialx = p.getData();
        //for (unsigned j = 0; j < trialx.size(); j++)
        //    printf("%lf ", trialx[j]);
        double y = mTolmin.Solve(trialx);
        //printf("\nNow y = %lf \n", y);
#pragma omp critical
        {
            if (y < mbesty)
            {
                bestx = trialx;
                mbesty = y;
            }
        }
    }
    mbesty = myProblem->funmin(bestx);

    this->allmeans.clear();
    this->allSamples.clear();
    if (kmeans != 0)
        delete kmeans;
}

MinCenter::~MinCenter()
{
}

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MinCenter(p);
}

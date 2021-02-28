#include "WMinCenter.h"

WMinCenter::WMinCenter(Problem *p)
    : Optimizer(p)
{
    addParameter("wmincenter_samples", "100", "Number of samples taken");
    addParameter("wmincenter_centers", "100", "Number of centers");
    addParameter("wmincenter_iterations", "100", "Number of iterations");
}

bool WMinCenter::terminated()
{
    return currentIteration >= iterations;
}

void WMinCenter::step()
{
    ++currentIteration;
    for (int i = 0; i < samples; i++)
    {
        std::vector<double> v = myProblem->getRandomPoint();
        Point point(i, v);
        allSamples.push_back(point);
    }
    wkmeans->run(allSamples);
}

void WMinCenter::init()
{
    currentIteration = 0;
    iterations = params["wmincenter_iterations"].toString().toInt();
    centers = params["wmincenter_centers"].toString().toInt();
    samples = params["wmincenter_samples"].toString().toInt();
    //minK = params["mincenter_minimum_centers"].toString().toInt();
    minK =5;
    int it = 100;
    wkmeans = new WKMeans(centers, it, minK);
    allmeans.resize(centers);
}

vector<Point> WMinCenter::meansWithFewPoints()
{
    std::vector<int> s = wkmeans->getSizes();
    for (unsigned i = 0; i < s.size(); i++)
    {
        if (s[i] < rate1)
        {
          //  wkmeans->deleteMean(i);
        }
    }
    wkmeans->run(allSamples);
    return wkmeans->getMeans();
}

vector<Point> WMinCenter::nearMeans(vector<Point> m)
{

    vector<Point> tmp = m;
    double min = Point::calcDistance(m.at(0), m.at(1));
    unsigned i, j, e = 0;
    double d = 0.0;
    min = 1e+100;
    for (i = 0; i < m.size(); i++)
    {
        for (j = i + 1; j < m.size(); j++)
        {
            double dis = Point::calcDistance(m.at(i), m.at(j));
            if (min > dis)
            {
                min = dis;
            }
        }
    }
    d = min;
    d *= rate2;
    for (i = 0; i < m.size(); i++)
        for (j = i + 1; j < m.size(); j++)
        {
            double distance = Point::calcDistance(m.at(i), m.at(j));
            if (distance <= d)
            {
                //tmp.erase(tmp.begin() + i);
                break;
            }
        }
    return tmp;
}

void WMinCenter::done()
{
/*
    int sum=0.0;
    std::vector<std::vector <Point> > ppoint = wkmeans->pointsOfMeans();
    for(int i=0; i<wkmeans->getSizes().size(); i++)
    {
        sum+=wkmeans->pointsOfMeans()[i].size();
        for(int j=0; j< wkmeans->pointsOfMeans()[i].size(); j++)
        {

            std::cout<<"Point "<< j <<" ( ";
            for (int e = 0; e < wkmeans->pointsOfMeans()[i][j].getDimensions(); e++)
            {
                std::cout<<wkmeans->pointsOfMeans()[i][j].getVal(e);
                std::cout<<" ";
            }
            std::cout<<")"<<std::endl;
        }
    }

    std::cout<<sum<<std::endl;
*/

    rate1 = (iterations * samples) / centers;
    rate1*=0.95;
    rate2= (double)centers * 4 / samples;
    Data bestx;
    double mbesty = 1e+100;
    printf("\nRATE1 => %d \n", rate1);
    printf("\nRATE2 => %lf \n", rate2);
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
    if (wkmeans != 0)
        delete wkmeans;
}

WMinCenter::~WMinCenter()
{
}

extern "C" WMINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new WMinCenter(p);
}

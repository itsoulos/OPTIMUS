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
    return currentIteration >= iterations;// || checkiterator();
    //return true;
}

void MinCenter::step()
{
    *omeans = *kmeans;
    ++currentIteration;
    //printf("--->%d\n", currentIteration);
    for (unsigned i = 0; i < allSamples.size(); i++)
        allSamples[i] = myProblem->getRandomPoint();
    kmeans->getNewSamples(allSamples);
    kmeans->run();
}

void MinCenter::init()
{
    currentIteration = 0;
    iterations = params["mincenter_iterations"].toString().toInt();
    centers = params["mincenter_centers"].toString().toInt();
    samples = params["mincenter_samples"].toString().toInt();
    kmeans = new KMeans();
    omeans = new KMeans();
    kmeans->setClusters(centers);
    allSamples.resize(samples);
    for (int i = 0; i < samples; i++)
        allSamples[i] = (Point)myProblem->getRandomPoint();
    kmeans->init(allSamples);

}
bool MinCenter::checkiterator()
{
    double mm = 0.0;
    for (unsigned i = 0; i < kmeans->getMeans().size(); i++)
    {
        Point p = kmeans->getMeans()[i].data_;
        Point p2 = omeans->getMeans()[i].data_;
        Data dimension = (Data)p.data_;
        for (unsigned j = 0; j < dimension.size(); j++)
            mm += pow(p.data_[j] - p2.data_[j],2) + pow(p.data_[j] - p2.data_[j],2);
        mm = sqrt(mm);
        //printf("dif: %lf\n",mm);
        if (mm < 0.05)
            return true;
    }
    return false;
}
vector<Point> MinCenter::filterMeans(vector<Point> m)
{
    std::vector<std::vector<Point>> mPoints = kmeans->getMeansWithPoints();
    long sum=0;

    for (unsigned i = 0; i < mPoints.size(); i++){
        printf("Total: %d i => %d size %ld\n", 
				mPoints.size(),
		i, mPoints[i].size());
        if( mPoints[i].size() < rate ){
            	kmeans->deleteMean(i);
        }
        sum+=mPoints[i].size();
    }
    printf("--------------------> sum1 => %ld \n", sum);
    //kmeans->runUpdate();
    mPoints = kmeans->getMeansWithPoints();
    sum=0.0;
    for (unsigned i = 0; i < mPoints.size(); i++){
        printf("i => %d size %ld\n", i, mPoints[i].size());
        sum+=mPoints[i].size();
    }
    printf("--------------------> sum2 => %ld \n", sum);
    return kmeans->getMeans();
}
vector<Point> MinCenter::checkSameMeans(vector<Point> m)
{
    vector<Point> tmp = m;
    double min = Point::distance(m.at(0), m.at(1));
    unsigned i, j, e = 0;
    double d = 0.0;
    //double max=1e-12;
	min = 1e+100;
    for (i = 0; i < m.size(); i++)
    {
        for (j = i + 1; j < m.size(); j++)
        {
            double dis = Point::distance(m.at(i), m.at(j));
            if (min > dis){
                min = dis;
            }
        }
    }
	d = min;
	d = d * 2;
    for (i = 0; i < m.size(); i++)
        for (j = i + 1; j < m.size(); j++)
        {
            double distance = Point::distance(m.at(i), m.at(j));
	//	printf("Distance is %lf and criterion is %lf \n",distance,d);
            //printf("distance: %lf min: %lf max: %lf\ mo: %lf \n", Point::distance(m.at(i),m.at(j)),min,max,d);
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

	rate =  (iterations * samples) / centers;
	rate = rate /4;
    Data bestx;
    double mbesty = 1e+100;
    vector<Point> tmp = filterMeans(kmeans->getMeans());
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("Init samples => %d \n", samples);
    printf("Init centers => %d \n", centers);
    printf("rate => %d \n", rate);
    printf("CENTERS (subtract means with 0 or a few points) ==> %ld\n", centers - allmeans.size());
    tmp = checkSameMeans(allmeans);
    int k=allmeans.size();
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("CENTERS (nearby means) ==> %ld\n", k - allmeans.size());
    printf("TOTAL CENTERS ==> %ld\n", allmeans.size());
    //allmeans = kmeans->getMeans();
#pragma omp parallel for num_threads(threads)
    for (unsigned int i = 0; i < allmeans.size(); i++)
    {
        Point p = allmeans[i];
        Tolmin mTolmin(myProblem);
        Data trialx = p.data_;
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
    if (kmeans != 0)
        delete kmeans;
    if (omeans != 0)
        delete omeans;
}

MinCenter::~MinCenter()
{
}

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MinCenter(p);
}

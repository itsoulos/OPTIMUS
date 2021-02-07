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

    //omeans = *kmeans;
    ++currentIteration;
    //printf("--->%d\n", samples);

    for (int i = 0; i < samples; i++){
        std::vector<double> v = myProblem->getRandomPoint();   
        Point point(i, v);
        allSamples.push_back(point);
    }
    /*
    for (int i = 0; i < allSamples.size(); i++){
        for (int j = 0; j < allSamples[i].getDimensions(); j++){
            printf("point %d---->%lf \n",i , allSamples[i].getVal(j));
        }
        printf("\n");
    }
    */
    kmeans->run(allSamples);
}

void MinCenter::init()
{
    currentIteration = 0;
    iterations = params["mincenter_iterations"].toString().toInt();
    centers = params["mincenter_centers"].toString().toInt();
    samples = params["mincenter_samples"].toString().toInt();
    int it=100;
    kmeans = new KMeans(centers,it);
    //omeans = new KMeans(centers,it);
    allmeans.resize(centers);
    //printf("allmeans size %d \n",allmeans.size());
}
/*
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
*/

vector<Point> MinCenter::filterMeans()
{


    std::vector<std::vector<Point>> mPoints = kmeans->pointsOfMeans();
    long sum=0;

    for (unsigned i = 0; i < mPoints.size(); i++){
        //printf("Total: %d i => %d size %ld\n",mPoints.size(),i, mPoints[i].size());
        if( mPoints[i].size() < rate ){
                //kmeans->deleteMean(i);
        }
        sum+=mPoints[i].size();
    }
    printf("--------------------> sum1 => %ld \n", sum);
    //kmeans->runUpdate();
    mPoints = kmeans->pointsOfMeans();
    sum=0.0;
    for (unsigned i = 0; i < mPoints.size(); i++){
        printf("i => %d size %ld\n", i, mPoints[i].size());
        sum+=mPoints[i].size();
    }
    printf("--------------------> sum2 => %ld \n", sum);

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
    rate = rate / 2;
    Data bestx;
    double mbesty = 1e+100;

    vector<Point> tmp = filterMeans();
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("\nInit samples => %d \n", samples);
    printf("Init centers => %d \n", centers);
    printf("rate => %d \n", rate);
    printf("CENTERS (subtract means with 0 or a few points) ==> %ld\n", centers - allmeans.size());
    tmp = nearMeans(allmeans);
    int k=allmeans.size();
    allmeans.clear();
    copy(tmp.begin(), tmp.end(), back_inserter(allmeans));
    printf("CENTERS (nearby means) ==> %ld\n", k - allmeans.size());
    printf("TOTAL CENTERS ==> %ld\n", allmeans.size());
/*
    std::vector<Point> vp;
    vp.resize(centers);
    vp=kmeans->getMeans();
    printf("\n");
    for (int i = 0; i < vp.size(); i++){
        printf("Cluster %d \n",i+1);
        for (int j = 0; j < vp[i].getDimensions(); j++){
            printf(" %lf \n",i , vp.at(i).getVal(j));
        }
        printf("\n");
    }
*/
    //allmeans = kmeans->getMeans();

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


    if (kmeans != 0)
        delete kmeans;
    //if (omeans != 0)
    //    delete omeans;

}

MinCenter::~MinCenter()
{
}

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MinCenter(p);
}

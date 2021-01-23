#include "mincenter.h"

MinCenter::MinCenter(Problem *p)
    : Optimizer(p)
{
    addParameter("mincenter_samples", "500", "Number of samples taken");
    addParameter("mincenter_centers", "50", "Number of centers");
    addParameter("mincenter_iterations", "200", "Number of iterations");
}

bool MinCenter::terminated()
{
    return currentIteration >= iterations;
}

void MinCenter::step()
{
    ++currentIteration;
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
    kmeans->setClusters(centers);
    allSamples.resize(samples);
    for (int i = 0; i < samples; i++)
        allSamples[i] = myProblem->getRandomPoint();
    kmeans->init(allSamples);
    omeans=kmeans->getMeans();

}
bool MinCenter::checkiterator(){
  /*
    double mm = 0.0;
    for (unsigned i = 0; i < allmeans.size() - 1; i++)
    {
        for (unsigned j = 0; j < allmeans[i].size(); j++)
        {
            mm += (allmeans[i] - omeans[i]) * (allmeans[i][j] - omeans[i][j]) + (allmeans[i][j] - omeans[i][j]) * (allmeans[i][j] - omeans[i][j]);
        }
        mm = fabs(sqrt(mm));
        if (mm < 0.001)
            return true;
    }*/
    return false;
}
vector<Point> MinCenter::checkSameMeans(vector<Point> m){
    vector<Point> tmp;
	double minDist=1e+100;
	vector<int> ok;
	ok.resize(m.size());
    for (unsigned i = 0; i < m.size(); i++)
	{
		ok[i]=1;
        for (unsigned j=i+1 ; j < m.size(); j++)
	{
		double d=Point::distance(m.at(i),m.at(j));
		if(d<minDist) minDist=d;
    //        	if (d<0.27)
    //            tmp.erase(tmp.begin()+i);
        }
}
    for (unsigned i = 0; i < m.size(); i++)
	{
		bool found=false;
        	for (unsigned j=i+1 ; j < m.size(); j++)
		{
			double d=Point::distance(m.at(i),m.at(j));
			if(d<  5  * minDist) ok[j]=false;
		
		}	
	}
	for(int i=0;i<m.size();i++) 
		if(ok[i])
			tmp.push_back(m.at(i));
		
    return tmp;
}
void MinCenter::done()
{
    Data bestx;
    double mbesty = 1e+100;    
    vector<Point> tmp = checkSameMeans(kmeans->getMeans());
    allmeans.clear();
	allmeans=tmp;
    printf("NumberOfMeans => %ld\n",allmeans.size());
    //allmeans = kmeans->getMeans();
#pragma omp parallel for num_threads(threads)
    for (unsigned int i = 0; i < allmeans.size(); i++)
    {
        Point p = allmeans[i];
        Tolmin mTolmin(myProblem);
        Data trialx = p.data_;
	for(int j=0;j<trialx.size();j++) printf("%lf ",trialx[j]);
        double y = mTolmin.Solve(trialx);
	printf("\nNow y = %lf \n",y);
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
}

MinCenter::~MinCenter()
{
}

extern "C" MINCENTER_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new MinCenter(p);
}

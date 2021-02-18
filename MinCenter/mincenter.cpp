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
        if (s[i] < rate1)
        {
            kmeans->deleteMean(i);
        }
    }
    kmeans->run(allSamples);
    return kmeans->getMeans();
}

vector<Point> MinCenter::nearMeans(vector<Point> m)
{
	vector<Data> distanceArray;
	distanceArray.resize(m.size());
	for(int i=0;i<m.size();i++) 
	{
		distanceArray[i].resize(m.size());
		for(int j=0;j<m.size();j++)
			distanceArray[i][j]=0.0;
	}

    vector<Point> tmp = m;
    double min = Point::distance(m.at(0), m.at(1));
    unsigned i, j, e = 0;
    double d = 0.0;
    double max=0.0;
    min = 1e+100;
    double sumDist=0.0;
    int countDist=0;
    for (i = 0; i < m.size(); i++)
    {
        for (j = i + 1; j < m.size(); j++)
        {
            double dis = Point::distance(m.at(i), m.at(j));
		distanceArray[i][j]=dis;
	    sumDist+=dis;
	    countDist++;
	    if(dis>max) max=dis;
            if (min > dis)
            {
                min = dis;
            }
        }
    }
    d = min;
    min/=max;
    //d *= 1.2;


    d *= rate2;
    sumDist=sumDist/countDist;
    sumDist/=max;
    d = min * 1.1;//was 1.25
	printf("Min: %.10lf Average: %.10lf Criterion: %.10lf \n",min,sumDist,d);
	vector<double> stdValue;
	stdValue.resize(m.size());
	for(int i=0;i<m.size();i++)
	{
		stdValue[i]=-1.0;
		int stdCount=0;
		for(int j=i+1;j<m.size();j++)
		{
			distanceArray[i][j]/=max;
			if(stdValue[i]<0) stdValue[i]=0.0;
			stdValue[i]+=pow(distanceArray[i][j]-min,2.0);
			stdCount++;
		}
		if(stdCount!=0) 
		{
			stdValue[i]/=stdCount;
			printf("STD[%4d]=%.5lf\n",i,stdValue[i]);
		}
		
	}
    vector<Point> newTemp;
	for(int i=0;i<tmp.size();i++)
		if(stdValue[i]>=0 && stdValue[i]>0.15)
			newTemp.push_back(tmp[i]);
	return newTemp;

    for (i = 0; i < m.size(); i++)
        for (j = i + 1; j < m.size(); j++)
        {
            double distance = Point::distance(m.at(i), m.at(j));
	    distance/=max;
	    Data x1=m.at(i).getData();
	    Data x2=m.at(i).getData();
            if (distance <= d && myProblem->getGradientCriterion(x1,x2))
            {
                tmp.erase(tmp.begin() + i);
                break;
            }
        }
    return tmp;
}

void MinCenter::done()
{
/*
    int sum=0.0;
    std::vector<std::vector <Point> > ppoint = kmeans->pointsOfMeans();
    for(int i=0; i<kmeans->getSizes().size(); i++)
    {
        sum+=kmeans->pointsOfMeans()[i].size();
        for(int j=0; j< kmeans->pointsOfMeans()[i].size(); j++)
        {

            std::cout<<"Point "<< j <<" ( ";
            for (int e = 0; e < kmeans->pointsOfMeans()[i][j].getDimensions(); e++)
            {
                std::cout<<kmeans->pointsOfMeans()[i][j].getVal(e);
                std::cout<<" ";
            }
            std::cout<<")"<<std::endl;
        }
    }

    std::cout<<sum<<std::endl;
*/

    rate1 = (iterations * samples) / centers;
    rate1*=0.85;
    rate2= (double)centers * 4 / samples;
    rate2 = 1.0 +centers / (3.0 * samples);
    rate2 = 1.0/3.0;
    Data bestx;
    double mbesty = 1e+100;
    //printf("\nRATE1 => %d \n", rate1);
    //printf("\nRATE2 => %lf \n", rate2);
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

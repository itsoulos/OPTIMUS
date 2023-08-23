#include "pdoublegenetic.h"

pDoubleGenetic::pDoubleGenetic(Problem *p)
    : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("double_chromosomes", "100", "Number of chromosomes");
    addParameter("double_generations", "200", "Number of generations");
    addParameter("double_selectionrate", "0.10", "Selection rate");
    addParameter("double_mutationrate", "0.05", "Mutation rate");
    addParameter("double_localsearchrate", "0.05", "Local search rate");
    addParameter("double_stoprule", "doublebox", "Stopping rule (doublebox,generations,stoponzero)");
    addParameter("double_debug", "no", "If it yes the full debug will be printed");
    addParameter("subClusterEnable", "1", "the number of subclusters that play a role in the termination rule: [1, islands]");
    addParameter("subCluster", "5", "number of subclusters for pDoubleGenetic");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("centers", "20", "centers");
    addParameter("pNumber", "5", "the number of samples for propagation");
    addParameter("parallelPropagateMethod", "1to1", "The propagation method used. Available values: 1to1,1toN,Nto1,NtoN");
    addParameter("propagateRate", "1", "The number of generations before the propagation takes place");
    addParameter("sample_method", "uniform", "Available values:: teams, kmeans, uniform");
}
bool sortByFirstDesc(const pair<double, int> &a, const pair<double, int> &b)
{
    return a.first < b.first;
}
void pDoubleGenetic::replace(int subClusterIndex, vector<pair<double, Data>> chrom)
{
    vector<pair<double, int>> tmp;
    tmp.resize(double_chromosomes);
    for (int i = 0; i < double_chromosomes; i++)
    {
        tmp.at(i).first = -1e+100;
        tmp.at(i).second = 0;
    }
    for (int i = subClusterStartPos(subClusterIndex), j = 0; i <= subClusterEndPos(subClusterIndex); i++, j++)
    {
        if (tmp.at(j).first < fitnessArray[i])
        {
            tmp.at(j).first = fitnessArray[i];
            tmp.at(j).second = i;
        }
    }
    sort(tmp.begin(), tmp.end(), sortByFirstDesc);
    tmp.resize(pNumber);
    vector<int> tmp2;
    for (int i = 0; i < pNumber; i++)
    {
        tmp2.push_back(tmp.at(i).second);
    }
    for (int i = subClusterStartPos(subClusterIndex); i <= subClusterEndPos(subClusterIndex); i++)
    {
        if (find(tmp2.begin(), tmp2.end(), i) != tmp2.end())
        {
            fitnessArray[i] = chrom.back().first;
            chromosome[i] = chrom.back().second;
            chrom.pop_back();
        }
    }
}

void    pDoubleGenetic::sendIslandToIsland(int from,int to)
{
    select(from);
    select(to);
    int start1 = subClusterStartPos(from);
    int end2   = subClusterEndPos(to);
    for(int i=0;i<pNumber;i++)
    {
        int pos1 = start1+i;
        int pos2 = end2-i;
        if(fitnessArray[pos1]<fitnessArray[pos2])
        {
            chromosome[pos2]=chromosome[pos1];
            fitnessArray[pos2]=fitnessArray[pos1];
        }
    }
}

void pDoubleGenetic::propagate()
{
    if (pNumber > 0 && subCluster > 1)
    {
        if (parallelPropagateMethod == "1to1")
        {
            int subCluster1, subCluster2;
            do
            {
                subCluster1 = rand() % subCluster;
                subCluster2 = rand() % subCluster;
            } while (subCluster1 == subCluster2);

            sendIslandToIsland(subCluster1,subCluster2);
         /*   vector<pair<double, Data>> tmp;
            tmp.resize(double_chromosomes);

            for (int i = 0; i < double_chromosomes; i++)
                tmp.at(i).first = 1e+100;

            for (int i = subClusterStartPos(subCluster1), j = 0; i <= subClusterEndPos(subCluster1); i++, j++)
            {
                if (tmp.at(j).first > fitnessArray[i])
                {
                    tmp.at(j).first = fitnessArray[i];
                    tmp.at(j).second = chromosome[i];
                }
            }
            sort(tmp.begin(), tmp.end());
            tmp.resize(pNumber);
            replace(subCluster2, tmp);*/
        }
        else if (parallelPropagateMethod == "1toN")
        {
            int subCluster1 = rand() % subCluster;
            vector<pair<double, Data>> tmp;

            for (int i = 0; i < subCluster; i++)
            {
                tmp.resize(double_chromosomes);
                for (int k = 0; k < double_chromosomes; k++)
                    tmp.at(k).first = 1e+100;
                if (i == subCluster1)
                    continue;
                sendIslandToIsland(subCluster1,i);
                /*
                for (int i = subClusterStartPos(subCluster1), j = 0; i <= subClusterEndPos(subCluster1); i++, j++)
                {
                    if (tmp.at(j).first > fitnessArray[i])
                    {
                        tmp.at(j).first = fitnessArray[i];
                        tmp.at(j).second = chromosome[i];
                    }
                }
                sort(tmp.begin(), tmp.end());
                tmp.resize(pNumber);
                replace(i, tmp);*/
            }
        }
        else if (parallelPropagateMethod == "Nto1")
        {
            int subCluster2 = rand() % subCluster;
            vector<pair<double, Data>> tmp;
            for (int i = 0; i < subCluster; i++)
            {
                tmp.resize(double_chromosomes);
                for (int k = 0; k < double_chromosomes; k++)
                    tmp.at(k).first = 1e+100;
                if (i == subCluster2)
                    continue;
                int subCluster1 = i;
                sendIslandToIsland(subCluster1,subCluster2);
                /*
                for (int k = subClusterStartPos(subCluster1), j = 0; k <= subClusterEndPos(subCluster1); k++, j++)
                {
                    if (tmp.at(j).first > fitnessArray[k])
                    {
                        tmp.at(j).first = fitnessArray[k];
                        tmp.at(j).second = chromosome[k];
                    }
                }
                sort(tmp.begin(), tmp.end());
                tmp.resize(pNumber);
                replace(subCluster2, tmp);*/
            }
        }
        else if (parallelPropagateMethod == "NtoN")
        {
            vector<pair<double, Data>> tmp;
            for (int i = 0; i < subCluster; i++)
            {
                for (int j = 0; j < subCluster; j++)
                {
                    tmp.resize(double_chromosomes);
                    for (int k = 0; k < double_chromosomes; k++)
                        tmp.at(k).first = 1e+100;
                    if (i == j)
                        continue;
                    int subCluster1 = i;
                    int subCluster2 = j;
                    sendIslandToIsland(subCluster1,subCluster2);
                    /*
                    for (int k = subClusterStartPos(subCluster1), e = 0; k <= subClusterEndPos(subCluster1); k++, e++)
                    {
                        if (tmp.at(e).first > fitnessArray[k])
                        {
                            tmp.at(e).first = fitnessArray[k];
                            tmp.at(e).second = chromosome[k];
                        }
                    }
                    sort(tmp.begin(), tmp.end());
                    tmp.resize(pNumber);
                    replace(subCluster2, tmp);*/
                }
            }
        }
    }
}
double pDoubleGenetic::randMToN(double M, double N)
{
    double r = rand() * 1.0 / RAND_MAX;
    return M + r * (N - M);
}
int pDoubleGenetic::randMToN(int M, int N)
{

     //std::random_device rd; // obtain a random number from hardware
     //std::mt19937 gen(rd()); // seed the generator
     //std::uniform_int_distribution<> distr(M, N);
     //return distr(gen);
    return (rand() % (N - M + 1)) + M;

}
int pDoubleGenetic::subClusterStartPos(int subClusterIndex)
{
    return subClusterIndex * population / subCluster;
}

int pDoubleGenetic::subClusterEndPos(int subClusterIndex)
{
    return (subClusterIndex + 1) * population / subCluster - 1;
}

void pDoubleGenetic::doubleCrossover(Data &parent1, Data &parent2,
                                     Data &children1, Data &children2)
{
    for (int i = 0; i < (int)parent1.size(); i++)
    {
        double x1 = parent1[i];
        double x2 = parent2[i];
        double alfa = -0.5 + 2.0 * myProblem->randomDouble();
        double g1 = alfa * x1 + (1.0 - alfa) * x2;
        double g2 = alfa * x2 + (1.0 - alfa) * x1;
        if (g1 > rmargin[i] || g1 < lmargin[i])
            g1 = x1;
        if (g2 > rmargin[i] || g2 < lmargin[i])
            g2 = x2;
        children1[i] = g1;
        children2[i] = g2;
    }
}

void pDoubleGenetic::getTournamentElement(int subCluster, Data &x)
{

    const int tournament_size = (double_chromosomes <= 100) ? 4 : 10;
    double max_fitness = 1e+100;
    int max_index = -1;
    for (int j = 0; j < tournament_size; j++)
    {
        int r = randomChromosome(subCluster);
        if (j == 0 || fitnessArray[r] < max_fitness)
        {
            max_index = r;
            max_fitness = fitnessArray[r];
        }
    }

    x = chromosome[max_index];
}

void pDoubleGenetic::tournament(int subCluster, Data &p1, Data &p2)
{
    getTournamentElement(subCluster, p1);
    getTournamentElement(subCluster, p2);
}

int pDoubleGenetic::randomChromosome(int subCluster)
{
    int pos = randMToN(subClusterStartPos(subCluster), subClusterEndPos(subCluster));
    // printf("subCluster(%d)pos = %d\n",subCluster, pos);
    return pos;
    //return gen1.bounded(subClusterStartPos(subCluster), subClusterEndPos(subCluster));
}

bool pDoubleGenetic::terminated()
{

    int c = 0;
    // #pragma omp parallel for private(i) num_threads(subCluster)
    for (int i = 0; i < subCluster; i++)
        if (this->checkSubCluster(i))
            c++;
    return generation >= double_generations || c >= subClusterEnable;
    // return generation >= double_generations;
}
bool pDoubleGenetic::checkSubCluster(int subClusterName)
{

	QString stop_method=params["double_stoprule"].toString();
    double difference = fabs(bestF2xInClusterOLD.at(subClusterName) - bestF2xInCluster.at(subClusterName));
     printf("%d] F2x(%d) : %f F2xOLD.at(%d) : %f different  : %f\n", generation, subClusterName, bestF2xInCluster.at(subClusterName), subClusterName, bestF2xInClusterOLD.at(subClusterName), subClusterName, difference);

	if(stop_method == "similarity")
	{
    //  if (bestF2xInClusterOLD.at(subClusterName) == bestF2xInCluster.at(subClusterName))
    if (difference < 1e-8)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
        return true;
    else return false;
	}

           //double bestValue = fabs(dmin);
           double bestValue = fabs(1.0+bestF2xInCluster.at(subClusterName));
           doublebox_xx1.at(subClusterName) += bestValue;
           doublebox_xx2.at(subClusterName) += bestValue * bestValue;
           doublebox_variance.at(subClusterName) = doublebox_xx2.at(subClusterName) / (generation + 1) - (doublebox_xx1.at(subClusterName) / (generation + 1)) * (doublebox_xx1.at(subClusterName) / (generation + 1));
           if (bestF2xInCluster.at(subClusterName) < bestF2xInClusterOLD.at(subClusterName) ||  generation<=1)
           {
               bestF2xInClusterOLD.at(subClusterName) = bestF2xInCluster.at(subClusterName);
               doublebox_stopat.at(subClusterName) = doublebox_variance.at(subClusterName) / 2.0;

           }
       if(doublebox_stopat.at(subClusterName)<1e-9)
           {
           doublebox_stopat.at(subClusterName)=doublebox_variance.at(subClusterName)/2.0;
       }


             //  printf("%4d] doublebox_variance.at(%d) : %f doublebox_stopat.at(%d) : %f different  : %lf\n", generation, subClusterName, doublebox_variance.at(subClusterName), subClusterName, doublebox_stopat.at(subClusterName), subClusterName, fabs(doublebox_variance.at(subClusterName) - doublebox_stopat.at(subClusterName)));
           return generation >= double_generations || (doublebox_variance.at(subClusterName) <= doublebox_stopat.at(subClusterName) && generation>=20);
   
    return false;
}
void pDoubleGenetic::step()
{
    if (generation)
    {
        #pragma omp parallel for  num_threads(subCluster)
        for (int t = 0; t < subCluster; t++)
        {
            mutate(t);
        }
    }

    double rate = params["double_localsearchrate"].toString().toDouble();
    dmin = 1e+100;
    QString debug = params["double_debug"].toString();

#pragma omp parallel for  num_threads(subCluster)
    for (int t = 0; t < subCluster; t++)
    {
        bestF2xInClusterOLD.at(t) = bestF2xInCluster.at(t);
        for (int i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {
            fitnessArray[i] = myProblem->funmin(chromosome[i]);
            if (rate > 0 && myProblem->randomDouble() <= rate)
            {
                fitnessArray[i] = localSearch(chromosome[i]);
            }
            if (bestF2xInCluster.at(t) > fitnessArray[i])
            {
                    bestF2xInCluster.at(t) = fitnessArray[i];
            }
        }
        // this->checkSubCluster(t);
    }
    if (pNumber > 0 && propagateRate > 0.0 && subCluster > 1)
    {
        if (generation % propagateRate)
            propagate();
    }

#pragma omp parallel for  num_threads(subCluster)
    for (int t = 0; t < subCluster; t++)
    {
        select(t);
    }



#pragma omp parallel for  num_threads(subCluster)
    for (int t = 0; t < subCluster; t++)
    {
        crossover(t);
    }

    for (int k = 0; k < subCluster; k++)
        newSum.at(k) = accumulate(&fitnessArray.at(this->subClusterStartPos(k)), &fitnessArray.at(this->subClusterEndPos(k)), 0);

    for (int k = 0; k < subCluster; k++)
    {
        newMO.at(k) = (double)newSum.at(k) / subCluster;
    }

    ++generation;
}

void    pDoubleGenetic::crossover(int subClusterIndex)
{
    int nchildren = (int)((1.0 - selection_rate) * double_chromosomes);
    if (!(nchildren % 2 == 0))
        nchildren++;
    Data parent0, parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    int start = subClusterStartPos(subClusterIndex) ;
    int end =subClusterEndPos(subClusterIndex);
    for (int count_children = 0, ii = 0; count_children <= nchildren; count_children += 2, ii += 1)
    {
        tournament(subClusterIndex, parent0, parent1);
        doubleCrossover(parent0, parent1,
                        children[start +count_children],
                        children[start+count_children + 1]);
    }
    for (int ii = 0; ii < nchildren; ii++)
    {
        chromosome[end - ii] = children[start + ii];
    }
}
void    pDoubleGenetic::select(int subClusterIndex)
{
    Data itemp;
    double dtemp;
    itemp.resize(myProblem->getDimension());
    for (int i = 0; i < double_chromosomes; i++)
    {
        for (int j = subClusterStartPos(subClusterIndex); j <= subClusterEndPos(subClusterIndex) - 1; j++)
        {
            if (fitnessArray[j + 1] < fitnessArray[j])
            {
                itemp = chromosome[j];
                chromosome[j] = chromosome[j + 1];
                chromosome[j + 1] = itemp;
                dtemp = fitnessArray[j];
                fitnessArray[j] = fitnessArray[j + 1];
                fitnessArray[j + 1] = dtemp;
            }
        }
    }
}

void    pDoubleGenetic::mutate(int subClusterIndex)
{
    for (int i = subClusterStartPos(subClusterIndex) + 1; i <= subClusterEndPos(subClusterIndex); i++)
    {

        for (int j = 0; j < myProblem->getDimension(); j++)
        {
            double r = myProblem->randomDouble();
            if (r <= mutation_rate)
            {
                double pc = 0.05;
                double newValue = chromosome[i][j]*pc*myProblem->randomDouble();
                double direction = myProblem->randomDouble()<=0.5?-1:1;
                chromosome[i][j]+=direction * newValue;
                if(chromosome[i][j]<lmargin[j])
                    chromosome[i][j]=lmargin[j];
                if(chromosome[i][j]>rmargin[j])
                    chromosome[i][j]=rmargin[j];
            }
        }
    }
}

vector<Data> pDoubleGenetic::runKmeans(int pop, int K)
{
    vector<Data> point;
    point.resize(pop);
    vector<Data> center;
    center.resize(K);
    vector<Data> copyCenter;
    copyCenter.resize(K);

    vector<int> belong;
    belong.resize(pop);
    vector<int> teamElements;
    teamElements.resize(K);
 for(int i=0;i<K;i++)
 {

     teamElements[i]=0;
 }

    for(int i=0;i<pop;i++)
    {
        point[i]=myProblem->getRandomPoint();
        belong[i]=rand() % K;
        teamElements[belong[i]]++;
    }

    for(int i=0;i<K;i++)
    {

        center[i].resize(myProblem->getDimension());
        for(int j=0;j<myProblem->getDimension();j++)
            center[i][j]=0.0;
    }
    for(int j=0;j<point.size();j++)
        {
         for(int k=0;k<myProblem->getDimension();k++)
         {
             center[belong[j]][k]+=point[j][k];
         }
     }
    for(int i=0;i<K;i++)
    {
        for(int j=0;j<myProblem->getDimension();j++)
            center[i][j]/=teamElements[i]>1?teamElements[i]:1;
    }


    int iteration = 1;
    double oldDist = 1e+100;
    while(true)
    {
        copyCenter = center;
        for(int i=0;i<K;i++) teamElements[i]=0;

        for(int i=0;i<point.size();i++)
        {
            int minCenterIndex = -1;
            double minCenterDist = 1e+100;
            for(int j=0;j<K;j++)
            {
                double d = getDistance(point[i],center[j]);
                if(d<minCenterDist)
                {
                    minCenterDist = d;
                    minCenterIndex = j;
                }
            }
            belong[i]=minCenterIndex;
            teamElements[minCenterIndex]++;

        }

        for(int i=0;i<K;i++)
        {
           for(int j=0;j<myProblem->getDimension();j++)
               center[i][j]=0.0;
        }
           for(int j=0;j<point.size();j++)
           {
            for(int k=0;k<myProblem->getDimension();k++)
            {
                center[belong[j]][k]+=point[j][k];
            }
           }

           for(int i=0;i<K;i++)
            for(int k=0;k<myProblem->getDimension();k++)
               center[i][k]/=teamElements[i]>1?teamElements[i]:1;

        double totalDistance = 0.0;
        for(int i=0;i<K;i++)
        {
            totalDistance+=getDistance(center[i],copyCenter[i]);
        }
        if(totalDistance<1e-6) break;
        if(iteration>1 && fabs(totalDistance-oldDist)<1e-8) break;
        iteration++;
        oldDist = totalDistance;
    }


    return center;
}

void pDoubleGenetic::init()
{
    rate = params["double_localsearchrate"].toString().toDouble();
    debug = params["double_debug"].toString();
    sample_method = params["sample_method"].toString();
    propagateRate = params["propagateRate"].toString().toDouble();
    parallelPropagateMethod = params["parallelPropagateMethod"].toString();
    pNumber = params["pNumber"].toString().toInt();
    mutation_rate = params["double_mutationrate"].toString().toDouble();
    selection_rate = params["double_selectionrate"].toString().toDouble();
    centers = params["centers"].toString().toInt();
    similarityMaxCount = params["similarityMaxCount"].toString().toInt();
    double_chromosomes = params["double_chromosomes"].toString().toInt();
    double_generations = params["double_generations"].toString().toInt();
    subCluster = params["subCluster"].toString().toInt();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    similarityCurrentCount.resize(subCluster);
    subClusterEnable = params["subClusterEnable"].toString().toInt();
    sum.resize(subCluster);
    newSum.resize(subCluster);
    MO.resize(subCluster);
    newMO.resize(subCluster);
    doublebox_xx1.resize(subCluster);
    doublebox_xx2.resize(subCluster);
    doublebox_variance.resize(subCluster);
    doublebox_stopat.resize(subCluster);
    oldBesty.resize(subCluster);
    bestF2xInCluster.resize(subCluster);
    bestF2xInClusterOLD.resize(subCluster);
    if (subClusterEnable > subCluster)
        subClusterEnable = subCluster;
    kmeans = NULL;

    if (sample_method == "teams")
    {
        population = double_chromosomes * subCluster;
        kmeans = new KMeans(subCluster, 30, centers);
        allSamples.clear();
        for (int i = 0; i < population; i++)
        {
            Point point(i, myProblem->getRandomPoint());
            allSamples.push_back(point);
        }

        kmeans->run(allSamples);

        allmeans.clear();
        chromosome.clear();
        fitnessArray.clear();

        std::vector<std::vector<Point>> ppoint = kmeans->pointsOfMeans();

        for (int i = 0; i < kmeans->pointsOfMeans().size(); i++)
        {
            for (int j = 0; j < kmeans->pointsOfMeans()[i].size(); j++)
            {
                Data s;
                for (int e = 0; e < ppoint[i][j].getDimensions(); e++)
                {
                    s.push_back(ppoint[i][j].getVal(e));
                }
                chromosome.push_back(s);
                fitnessArray.push_back(myProblem->funmin(s));
            }
        }
        // printf("teams: population = %d chrom= %d fitt %d centers= %d \n", population, chromosome.size(), fitnessArray.size(), centers);
    }

    else if (sample_method == "kmeans")
    {

        vector<Data> centerValues = runKmeans(double_chromosomes * subCluster,
                                              centers * subCluster);
	int currentCenters = 0;
	vector<Data> finalCenterValues;
	for(int i=0;i<centers;i++)
	{
		double dmin = 1e+100;
		int imin = -1;
		for(int j=0;j<i;j++)
		{
			if(i==j) continue;
			if(getDistance(centerValues[i],centerValues[j])<dmin)
			{
				dmin = getDistance(centerValues[i],centerValues[j]);
				imin = j;
			}
		}
		if(dmin>1e-8){
			finalCenterValues.push_back(centerValues[i]);
			currentCenters++;
		}
	}

	centers  = finalCenterValues.size();
        double_chromosomes = centers;
        population = double_chromosomes * subCluster;
        fitnessArray.resize(population);
        chromosome.resize(population);
        for(int i=0;i<population;i++)
        {
             chromosome[i]=finalCenterValues[i];
             fitnessArray[i]=myProblem->funmin(chromosome[i]);
        }
        /*
        int pop = double_chromosomes * subCluster;



        kmeans = new KMeans(pop, 30, centers);

        allSamples.clear();
        for (int i = 0; i < pop; i++)
        {
            Point point(i, myProblem->getRandomPoint());
            allSamples.push_back(point);
        }

        kmeans->run(allSamples);

        vector<Point> tmp = kmeans->getMeans();

        allmeans.clear();

        copy(tmp.begin(), tmp.end(), back_inserter(allmeans));

        double_chromosomes = centers;
        population = double_chromosomes * subCluster;


        fitnessArray.resize(population);
        chromosome.resize(population);

        for (int i = 0; i < population; i++)
        {

         Point p = allmeans[i];
            chromosome[i].resize(myProblem->getDimension());

            chromosome[i] = (Data)p.getData();
            fitnessArray[i] = myProblem->funmin(chromosome[i]);
        }
*/
    }
    else if(sample_method=="uniform")
    {
        population = double_chromosomes * subCluster;
        fitnessArray.resize(population);
        chromosome.resize(population);
        for (int i = 0; i < population; i++)
        {
            chromosome[i] = myProblem->getRandomPoint();
            fitnessArray[i] = myProblem->funmin(chromosome[i]);
        }
        // printf("uniform: population = %d chrom= %d fitt %d centers= %d \n", population, chromosome.size(), fitnessArray.size(), centers);
    }
    else
    {
        sampleFromProblem(centers,chromosome,fitnessArray);
        for(int i=0;i<centers;i++)
            fitnessArray[i]=myProblem->funmin(chromosome[i]);
        population = centers;
        double_chromosomes = centers;

    }
    for (int i = 0; i < subCluster; i++)
    {

        bestF2xInCluster.at(i) = 1e+100;
        similarityCurrentCount.at(i) = 0;
        sum.at(i) = 0.0;
        newSum.at(i) = 0.0;
        // MO.at(i) = 0.0;
        doublebox_xx1.at(i) = 0.0;
        doublebox_xx2.at(i) = 0.0;
        oldBesty.at(i) = 1e+100;
        for (int j = subClusterStartPos(i); j <= subClusterEndPos(i); j++)
        {
            if (bestF2xInCluster.at(i) > fitnessArray[j])
            {
                bestF2xInCluster.at(i) = fitnessArray[j];
            }
        }
    }

    children.resize(population);
    for (int i = 0; i < population; i++)
    {
        children[i].resize(myProblem->getDimension());
    }
    generation = 0;
}

void pDoubleGenetic::done()
{
    dmin = 1e+100;
    for(int i=0;i<population;i++)
    {
        if(fitnessArray[i]<dmin)
           {
            dmin = fitnessArray[i];
            bestx = chromosome[i];
        }
    }
    dmin = localSearch(bestx);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
    // std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
    this->allmeans.clear();
    this->allSamples.clear();
    if (kmeans != NULL)
        delete kmeans;
}

void pDoubleGenetic::Solve()
{
    Optimizer::Solve();
}

pDoubleGenetic::~pDoubleGenetic()
{
}

extern "C" PDOUBLEGENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new pDoubleGenetic(p);
}

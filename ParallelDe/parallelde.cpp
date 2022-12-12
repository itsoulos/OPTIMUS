#include "parallelde.h"

ParallelDe::ParallelDe(Problem *p)
    :Optimizer(p)
{
    addParameter("parde_agents", "100", "Number of population");
    addParameter("parde_generations", "1000", "Maximum number of generations");
    addParameter("parde_cr", "0.9", "Crossover Probability");
    addParameter("parde_f", "0.8", "Differential Weight");
    addParameter("parde_islands","10","Number of thread islands");
}


int     ParallelDe::islandStartPos(int islandIndex)
{
    return islandIndex*population.size()/islands;
}

int     ParallelDe::islandEndPos(int islandIndex)
{
    return (islandIndex+1)*population.size()/islands-1;
}


void    ParallelDe::init()
{
    generation = 0;
    islands = params["parde_islands"].toString().toInt();
    parde_F = params["parde_f"].toString().toDouble();
    parde_CR = params["parde_cr"].toString().toDouble();
    int agents = params["parde_agents"].toString().toInt();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    population.resize(agents);
    fitness_array.resize(agents);
    for(int i=0;i<agents;i++)
    {
        population[i]=myProblem->getRandomPoint();
        fitness_array[i]=myProblem->funmin(population[i]);
    }
    bestIslandIndex.resize(islands);
    bestIslandValues.resize(islands);
}

void    ParallelDe::propagateIslandValues()
{
    for(int i=0;i<islands;i++)
    {
        int randomIsland = rand()  % islands;
        if(randomIsland == i) continue;

        {

            //find worst value
            int minIndex =0;
            double minValue = -1e+100;
            for(int pos=islandStartPos(randomIsland);pos<=islandEndPos(randomIsland);pos++)
            {
                if(fitness_array[pos]>minValue)
                {
                    minIndex = pos;
                    minValue = fitness_array[pos];
                }
            }
            if(fitness_array[minIndex]>bestIslandValues[i])
            {
                fitness_array[minIndex]=bestIslandValues[i];
                population[minIndex]=population[bestIslandIndex[i]];
            }
        }
    }
}

int     ParallelDe::tournament(int islandIndex,int tsize)
{
    int bestIndex = 0;
    double bestValue = 1e+100;
    for(int i=0;i<tsize;i++)
    {
        int startPos = islandStartPos(islandIndex);
        int endPos = islandEndPos(islandIndex);
        int r = rand() % (endPos-startPos)+startPos;
        if(fitness_array[r]<bestValue || i==0)
        {
            bestValue = fitness_array[r];
            bestIndex = r;
        }
    }
    return bestIndex;
}

bool    ParallelDe::terminated()
{
   int parde_generations = params["parde_generations"].toString().toInt();
   printf("Generation=%d Max=%d\n",generation,parde_generations);
   for(int i=0;i<islands;i++)
   {
       printf("BEST ISLAND[%d]=%20.10lg\n",i,bestIslandValues[i]);
   }
   return generation>=parde_generations;
}

void    ParallelDe::step()
{

    ++generation;
#pragma omp parallel for num_threads(islands)
    for(int t=0;t<islands;t++)
    {
        bestIslandIndex[t]=0;
        bestIslandValues[t]=1e+100;
        for(int j=islandStartPos(t);j<=islandEndPos(t);j++)
        {
            double alfa = -0.5 + 2.0 * rand()*1.0/RAND_MAX;
            double differentialWeight = alfa;
            int indexA,indexB,indexC;
            do
            {
                indexA = tournament(t);
                indexB = tournament(t);
                indexC = tournament(t);
            }while(indexA==indexB || indexB==indexC || indexA==indexC);
            Data y;

            y.resize(myProblem->getDimension());
            int R = rand() % y.size();
            for(int k=0;k<y.size();k++)
            {
                int ri = rand()*1.0/RAND_MAX;
                if(ri<parde_CR || k==R)
                {
                    y[k]=population[indexA][k]+
                            differentialWeight*(population[indexB][k]-population[indexC][k]);
                    if(y[k]<lmargin[k] || y[k]>rmargin[k])
                        y[k]=population[j][k];
                }
                else
                {
                    y[k]=population[j][k];
                }
            }
            double yy = myProblem->funmin(y);
            if(yy<=fitness_array[j])
            {
                population[j]=y;
                fitness_array[j]=yy;
            }
            if(fitness_array[j]<bestIslandValues[t])
            {
                bestIslandIndex[t]=j;
                bestIslandValues[t]=fitness_array[j];
            }
        }

    }
    propagateIslandValues();
}

void    ParallelDe::done()
{
    int bestIndex = 0;
    double bestValue = 1e+100;
    for(int i=0;(int)i<population.size();i++)
    {
        if(fitness_array[i]<bestValue || i==0)
        {
            bestIndex = i;
            bestValue = fitness_array[i];
        }
    }
    bestValue = localSearch(population[bestIndex]);
}

ParallelDe::~ParallelDe()
{
    //nothing here
}


extern "C" PARALLELDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new ParallelDe(p);
}

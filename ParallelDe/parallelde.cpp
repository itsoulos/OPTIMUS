#include "parallelde.h"

ParallelDe::ParallelDe(Problem *p)
    :Optimizer(p)
{
    addParameter("parde_termination","similarity","Termination rule. Available values: maxiters,doublebox,similarity");
    addParameter("parde_similarityMax","10","Maximum allowed itearations for Similarity Stopping rule");
    addParameter("parde_agents", "20", "Number of population");
    addParameter("parde_generations", "1000", "Maximum number of generations");
    addParameter("parde_cr", "0.9", "Crossover Probability");
    addParameter("parde_weight_method","random","The differential weight method. Available values are: random, ali, constant");
    addParameter("parde_f", "0.8", "Differential Weight");
    addParameter("parde_islands","10","Number of thread islands");
    addParameter("parde_propagate_rate","5","The number of generations before the propagation takes place");
    addParameter("parde_selection_method","tournament","The selection method used. Available values are: tournament,random");
    addParameter("parde_propagate_method","1-1","The propagation method used. Available values: 1-1,1-N,N-1,N-N");
}

int     ParallelDe::selectAtom(int islandIndex)
{
    QString parde_selection_method = params["parde_selection_method"].toString();
    if(parde_selection_method == "random")
    {
        int a = islandStartPos(islandIndex);
        int b = islandEndPos(islandIndex);
        int r = a+rand() % (b-a);
        return r;
    }
    else return tournament(islandIndex);
}

void    ParallelDe::getBestValue(int &index,double &value)
{
    value = 1e+100;
    index = 0;
    for(int i=0;(int)i<population.size();i++)
    {
        if(fitness_array[i]<value || i==0)
        {

            value = fitness_array[i];
            index = i;
        }
    }

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
    similarity_best_value=1e+100;
    similarity_max_count=10;
    similarity_current_count=0;
    doublebox_xx1=0.0;
    doublebox_xx2=0.0;
    doublebox_best_value = 1e+100;
    doublebox_stopat = 1e+100;
    doublebox_variance= 1e+100;

    islands = params["parde_islands"].toString().toInt();
    parde_F = params["parde_f"].toString().toDouble();
    parde_CR = params["parde_cr"].toString().toDouble();
    int agents = params["parde_agents"].toString().toInt();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    population.resize(agents * islands);
    fitness_array.resize(agents * islands);
    for(int i=0;i<agents*islands;i++)
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

    double bestValue;
    int bestIndex;
   int parde_generations = params["parde_generations"].toString().toInt();
   getBestValue(bestIndex,bestValue);
   printf("Generation=%d Value=%20.10lf\n",generation,bestValue);
 /*  for(int i=0;i<islands;i++)
   {
       printf("BEST ISLAND[%d]=%20.10lg\n",i,bestIslandValues[i]);
   }*/
   QString parde_termination =  params["parde_termination"].toString();
   if(parde_termination=="maxiters")
    return generation>=parde_generations;
   else
   if(parde_termination == "similarity")
   {
       int parde_similarityMax = params["parde_similarityMax"].toString().toInt();
       similarity_max_count = parde_similarityMax;
        if(fabs(bestValue-similarity_best_value)>1e-5)
        {
            similarity_best_value=bestValue;
            similarity_current_count=0;
        }
        else similarity_current_count++;
        return (similarity_current_count>=similarity_max_count ||
                generation>=parde_generations);
   }
   else
   if(parde_termination=="doublebox")
   {
        doublebox_xx1+=bestValue;
        doublebox_xx2+=bestValue * bestValue;
        doublebox_variance = doublebox_xx2/generation-(doublebox_xx1/generation)*(doublebox_xx1/generation);
        if(bestValue<doublebox_best_value)
        {
            doublebox_best_value = bestValue;
            doublebox_stopat = doublebox_variance/2.0;
        }
        return (doublebox_variance<=doublebox_stopat || generation>=parde_generations);
   }
   else return false;
}

double  ParallelDe::getDifferentialWeight()
{
    QString parde_weight_method = params["parde_weight_method"].toString();
    if(parde_weight_method=="constant")
        return parde_F;
    else
    if(parde_weight_method=="ali")
    {

    }
    else
        return -0.5 + 2.0 * rand()*1.0/RAND_MAX;
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
            double differentialWeight = getDifferentialWeight();
            int indexA,indexB,indexC;
            do
            {
                indexA = selectAtom(t);
                indexB = selectAtom(t);
                indexC = selectAtom(t);
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
    int parde_propagate_rate = params["parde_propagate_rate"].toString().toInt();
    if(generation%parde_propagate_rate)
        propagateIslandValues();
}

void    ParallelDe::done()
{
    int bestIndex = 0;
    double bestValue = 1e+100;
    getBestValue(bestIndex,bestValue);
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

#include "parallelde.h"

ParallelDe::ParallelDe(Problem *p)
    : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("parde_termination", "similarity", "Termination rule. Available values: maxiters,doublebox,similarity");
    addParameter("parde_agents", "40", "Number of population");
    addParameter("parde_generations", "1000", "Maximum number of generations");
    addParameter("parde_cr", "0.9", "Crossover Probability");
    addParameter("parde_weight_method", "random", "The differential weight method. Available values are: random, ali, constant");
    addParameter("parde_f", "0.8", "Differential Weight");
    addParameter("parde_propagate_rate", "5", "The number of generations before the propagation takes place");
    addParameter("parde_selection_method", "random", "The selection method used. Available values are: tournament,random");
    addParameter("parde_propagate_method", "NtoN", "The propagation method used. Available values: 1to1,1toN,Nto1,NtoN");    
    addParameter("parde_similarityMax", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("parde_islands", "5", "Number of thread islands");
    addParameter("parde_islands_enable", "2", "the number of islands that play a role in the termination rule: [1, islands. O for global check.");
}

int ParallelDe::selectAtom(int islandIndex)
{
    QString parde_selection_method = params["parde_selection_method"].toString();
    if (parde_selection_method == "random")
    {
        int a = islandStartPos(islandIndex);
        int b = islandEndPos(islandIndex);
        int r = a + rand() % (b - a);
        return r;
    }
    else
        return tournament(islandIndex);
}

void ParallelDe::getBestValue(int &index, double &value)
{

    value = 1e+100;
    index = 0;
    for (int i = 0; (int)i < population.size(); i++)
    {
        if (fitness_array[i] < value || i == 0)
        {
            value = fitness_array[i];
            index = i;
        }
    }
}

void ParallelDe::getBestValue(int islandName, int &index, double &value)
{
	value = bestIslandValues[islandName];
	index = bestIslandIndex[islandName];
	return;
    value = 1e+100;
    index = 0;
    int i = islandStartPos(islandName);
    int end = this->islandEndPos(islandName);
    for ( ; (int)i <= end; i++)
    {
        if (fitness_array.at(i) < value)
        {
            value = fitness_array.at(i);
            index = i;
        }
    }
}

int ParallelDe::islandStartPos(int islandIndex)
{
    return islandIndex * population.size() / islands;
}

int ParallelDe::islandEndPos(int islandIndex)
{
    return (islandIndex + 1) * population.size() / islands - 1;
}

void ParallelDe::init()
{
    generation = 0;
    similarity_max_count = 10;
    global_sim_value = 1e+100;
    global_sim_count = 0;

    parde_generations = params["parde_generations"].toString().toInt();
    islands = params["parde_islands"].toString().toInt();
    doublebox_xx1.resize(islands);
    doublebox_xx2.resize(islands);
    doublebox_best_value.resize(islands);
    doublebox_stopat.resize(islands);
    doublebox_variance.resize(islands);
    for (int i=0;i<islands;i++)
    {
        doublebox_xx1.at(i) = 0.0;
        doublebox_xx2.at(i) = 0.0;
        doublebox_best_value.at(i) = 1e+100;
        doublebox_stopat.at(i) = 1e+100;
        doublebox_variance.at(i) = 1e+100;
    }

    parde_F = params["parde_f"].toString().toDouble();
    parde_CR = params["parde_cr"].toString().toDouble();
    agents = params["parde_agents"].toString().toInt();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    population.resize(agents * islands);
    fitness_array.resize(agents * islands);
    for (int i = 0; i < agents * islands; i++)
    {
        population[i] = myProblem->getRandomPoint();
        fitness_array[i] = myProblem->funmin(population[i]);
    }
    bestIslandIndex.resize(islands);
    bestIslandValues.resize(islands);
    similarity_max_count = params["parde_similarityMax"].toString().toInt();
    similarity_best_value.resize(islands);
    similarity_current_count.resize(islands);
    sum.resize(islands);
    newSum.resize(islands);
    MO.resize(islands);
    newMO.resize(islands);
    for (int i=0;i<islands;i++)
    {
        similarity_best_value.at(i) = 1e+100;
        similarity_current_count.at(i) = 0;
        //sum.at(j) = accumulate(fitness_array.at(this->islandStartPos(j)), fitness_array.at(this->islandEndPos(j)), 0);
        //sum.at(j) = accumulate(fitness_array.begin() + this->islandStartPos(j), fitness_array.begin() + this->islandEndPos(j), 0);
        //sum.at(j) = accumulate(this->islandStartPos(j), this->islandEndPos(j), 0);
        sum.at(i) = 0;
        MO.at(i) = 0;
    }

    parde_termination = params["parde_termination"].toString();
    parde_propagate_method = params["parde_propagate_method"].toString();
    parde_islands_enable = params["parde_islands_enable"].toString().toInt();
    parde_weight_method = params["parde_weight_method"].toString();
    parde_propagate_rate = params["parde_propagate_rate"].toString().toInt();
}

void ParallelDe::replaceValueInIsland(int islandIndex, Data &x, double &y)
{
    // find worst value
    int minIndex = 0;
    double minValue = -1e+100;
    for (int pos = islandStartPos(islandIndex); pos <= islandEndPos(islandIndex); pos++)
    {
        if (fitness_array[pos] > minValue)
        {
            minIndex = pos;
            minValue = fitness_array[pos];
        }
    }
    if (fitness_array[minIndex] > y)
    {
        fitness_array[minIndex] = y;
        population[minIndex] = x;
    }
}

void ParallelDe::propagateIslandValues()
{
    if (parde_propagate_method == "1to1")
    {
        int island1 = rand() % islands;
        int island2 = rand() % islands;
        if (island1 == island2)
            return;
        Data xx = population[bestIslandIndex[island1]];
        double yy = bestIslandValues[island1];
        replaceValueInIsland(island2, xx, yy);
    }
    else if (parde_propagate_method == "1toN")
    {
        int island1 = rand() % islands;
        Data xx = population[bestIslandIndex[island1]];
        double yy = bestIslandValues[island1];
        for (int i = 0; i < islands; i++)
        {
            if (i == island1)
                continue;
            replaceValueInIsland(i, xx, yy);
        }
    }
    else if (parde_propagate_method == "Nto1")
    {
        int island2 = rand() % islands;
        for (int i = 0; i < islands; i++)
        {
            if (i == island2)
                continue;
            int island1 = i;
            Data xx = population[bestIslandIndex[island1]];
            double yy = bestIslandValues[island1];
            replaceValueInIsland(island2, xx, yy);
        }
    }
    else if (parde_propagate_method == "NtoN")
    {
        for (int i = 0; i < islands; i++)
        {
            for (int j = 0; j < islands; j++)
            {
                if (i == j)
                    continue;
                int island1 = i;
                int island2 = j;
                Data xx = population[bestIslandIndex[island1]];
                double yy = bestIslandValues[island1];
                replaceValueInIsland(island2, xx, yy);
            }
        }
    }
}

int ParallelDe::tournament(int islandIndex, int tsize)
{
    int bestIndex = 0;
    double bestValue = 1e+100;
    for (int i = 0; i < tsize; i++)
    {
        int startPos = islandStartPos(islandIndex);
        int endPos = islandEndPos(islandIndex);
        int r = rand() % (endPos - startPos) + startPos;
        if (fitness_array[r] < bestValue || i == 0)
        {
            bestValue = fitness_array[r];
            bestIndex = r;
        }
    }
    return bestIndex;
}
bool ParallelDe::checkIsland(int islandName)
{
    double bestValue;
    int bestIndex;

    getBestValue(islandName, bestIndex, bestValue);
    //printf("Generation=%d Value=%20.10lf\n", generation, bestValue);
    if (parde_termination == "similarity")
    {
//---------------------SIMILARITY CASE 1 AVERAGE TERMS

        double difference = fabs(newMO.at(islandName) - MO.at(islandName));
        //printf("%d] newMO.at(%d) : %f MO.at(%d) : %f different  : %lf\n", generation, islandName, newMO.at(islandName), islandName, MO.at(islandName), islandName, difference );
        MO.at(islandName) = newMO.at(islandName);

        if (difference < 1e-5)
            similarity_current_count.at(islandName)++;
        else
            similarity_current_count.at(islandName) = 0;
        if (similarity_current_count.at(islandName) >= similarity_max_count)
            return true;

//---------------------SIMILARITY CASE 2 QUANTITIES
/*
        double difference = fabs(newSum.at(islandName) - sum.at(islandName));
        printf("%d] neWsum.at(%d) : %f Sum.at(%d) : %f different  : %lf\n", generation, islandName, newSum.at(islandName), islandName, sum.at(islandName), islandName, difference );
        sum.at(islandName) = newSum.at(islandName);

        if (difference < 1e-5)
            similarity_current_count.at(islandName)++;
        else
            similarity_current_count.at(islandName) = 0;
        if (similarity_current_count.at(islandName) >= similarity_max_count)
            return true;
*/
//---------------------SIMILARITY CASE 3
/*
        if (fabs(bestValue - similarity_best_value.at(islandName)) > 1e-5)
        {
            similarity_best_value.at(islandName) = bestValue;
            similarity_current_count.at(islandName) = 0;
        }
        else
            similarity_current_count.at(islandName)++;
        return similarity_current_count.at(islandName) >= similarity_max_count;
*/
    }
    else if (parde_termination == "doublebox")
    {
	    double v1 = fabs(1.0+bestValue);
        doublebox_xx1.at(islandName) += v1;
        doublebox_xx2.at(islandName) += v1 * v1;
        doublebox_variance.at(islandName) = doublebox_xx2.at(islandName) / (generation+1) - (doublebox_xx1.at(islandName) / (generation+1)) * (doublebox_xx1.at(islandName) / (generation+1));
        if (bestValue < doublebox_best_value.at(islandName))
        {
            doublebox_best_value.at(islandName) = bestValue;
            doublebox_stopat.at(islandName) = doublebox_variance.at(islandName) / 2.0;
        }
        printf("%4d] doublebox_variance.at(%d) : %f doublebox_stopat.at(%d) : %f different  : %lf\n", generation,islandName, doublebox_variance.at(islandName),islandName, doublebox_stopat.at(islandName),islandName,fabs(doublebox_variance.at(islandName) - doublebox_stopat.at(islandName)));
        return doublebox_variance.at(islandName) <= doublebox_stopat.at(islandName) && generation>=10;
    }

    return false;
}
bool ParallelDe::terminated()
{
    int parde_islands_enable = params["parde_islands_enable"].toString().toInt();
    if(parde_islands_enable==0)
    {
        int index;
        double bestValue;
        getBestValue(index,bestValue);
        if(fabs(bestValue-global_sim_value)>1e-5)
        {
            global_sim_value = bestValue;
            global_sim_count=0;
        }
        else global_sim_count++;

        return generation >= parde_generations || global_sim_count>=similarity_max_count;
    }
    int c = 0;

    for (int i = 0; i < islands; i++)
        if (this->checkIsland(i))
            c++;
    //printf("c = %d islands = %d \n",c,islands);
    //printf("c = %d islands = %d parde_islands_enable = %d\n",c,islands,parde_islands_enable);
    return generation >= parde_generations || c >= parde_islands_enable;
}

double ParallelDe::getDifferentialWeight()
{

    if (parde_weight_method == "constant")
        return parde_F;
    else if (parde_weight_method == "ali")
    {
    }
    else
        return -0.5 + 2.0 * rand() * 1.0 / RAND_MAX;
}

void ParallelDe::step()
{
    ++generation;
    int t;
#pragma omp parallel for private(t) num_threads(islands)
    for (t = 0; t < islands; t++)
    {
        bestIslandIndex[t] = 0;
        bestIslandValues[t] = 1e+100;
        int j;
#pragma omp parallel for private(j) num_threads(islands)
        for (j = islandStartPos(t); j <= islandEndPos(t); j++)
        {
            double differentialWeight = getDifferentialWeight();
            int indexA, indexB, indexC;
            do
            {
                indexA = selectAtom(t);
                indexB = selectAtom(t);
                indexC = selectAtom(t);
            } while (indexA == indexB || indexB == indexC || indexA == indexC);
            Data y;

            y.resize(myProblem->getDimension());
            int R = rand() % y.size();
            for (int k = 0; k < y.size(); k++)
            {
                int ri = rand() * 1.0 / RAND_MAX;
                if (ri < parde_CR || k == R)
                {
                    y[k] = population[indexA][k] +
                           differentialWeight * (population[indexB][k] - population[indexC][k]);
                    if (y[k] < lmargin[k] || y[k] > rmargin[k])
                        y[k] = population[j][k];
                }
                else
                {
                    y[k] = population[j][k];
                }
            }
            double yy = myProblem->funmin(y);

            if (yy <= fitness_array[j])
            {
                population[j] = y;
                fitness_array[j] = yy;
            }

            if (fitness_array[j] < bestIslandValues[t])
            {
                bestIslandIndex[t] = j;
                bestIslandValues[t] = fitness_array[j];
            }

            //newSum.at(t) = accumulate(fitness_array.begin() + this->islandStartPos(t), fitness_array.begin() + this->islandEndPos(t), 0);
            //newSum.at(j) = accumulate(fitness_array.at(j), fitness_array.begin(j) + this->islandEndPos(j), 0);
            //this->checkIsland(t);
        }
        //newSum.at(t) = accumulate(fitness_array.begin() + this->islandStartPos(t), fitness_array.begin() + this->islandEndPos(t), 0);
        //this->checkIsland(t);
    }

    for (int j = 0; j < islands; j++)
        newSum.at(j) = accumulate(&fitness_array.at(this->islandStartPos(j)), &fitness_array.at(this->islandEndPos(j)), 0);

    for (int j = 0; j < islands; j++)
    {
        newMO.at(j) = (double)newSum.at(j)/islands;
        this->checkIsland(j);
    }
    if (generation % parde_propagate_rate)
        propagateIslandValues();
}

void ParallelDe::done()
{
    int bestIndex = 0, bestI = 0;
    double bestValue = 1e+100, bestV = 1e+100;
    getBestValue(bestIndex, bestValue);
/*
    for (int i = 0;i < islands; i++)
    {
        this->getBestValue(i,bestI, bestV);
        if (bestV<bestValue)
        {
            bestValue = bestV;
            bestIndex = bestI;
        }
    }
    */
    bestValue = localSearch(population[bestIndex]);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
//    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
}

ParallelDe::~ParallelDe()
{
    // nothing here
}

extern "C" PARALLELDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new ParallelDe(p);
}

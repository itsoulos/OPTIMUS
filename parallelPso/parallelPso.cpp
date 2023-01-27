#include "parallelPso.h"

parallelPso::parallelPso(Problem *p) : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("parallelPsoParticles", "50", "Number of pso particles");
    addParameter("parallelPsoGenerations", "200", "Maximum number of pso generations");
    addParameter("parallelPsoC1", "0.5", "Pso c1 parameter");
    addParameter("parallelPsoC2", "0.5", "Pso c2 parameter");
    addParameter("psoLocalsearchRate", "0.01", "Local search rate for pso");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("pardePropagateRate", "2", "The number of generations before the propagation takes place");
    addParameter("parallelPropagateMethod", "1toN", "The propagation method used. Available values: 1to1,1toN,Nto1,NtoN");
    addParameter("subCluster", "10", "number of subclusters for pso");
    addParameter("subClusterEnable", "1", "the number of subclusters that play a role in the termination rule: [1, islands");
}

void parallelPso::getBestValue(int &index, double &value)
{
    value = 1e+100;
    index = 0;
    for (int i = 0; (int)i < particles.size(); i++)
    {
        if (fitness_array[i] < value || i == 0)
        {
            value = fitness_array[i];
            index = i;
        }
    }
}

void parallelPso::getBestValue(int subClusterName, int &index, double &value)
{
    value = 1e+100;
    index = 0;
    int i = subClusterStartPos(subClusterName);
    int end = this->subClusterEndPos(subClusterName);
    for (; (int)i < end; i++)
    {
        if (fitness_array.at(i) < value)
        {
            value = fitness_array.at(i);
            index = i;
        }
    }
}

int parallelPso::subClusterStartPos(int subClusterIndex)
{
    return subClusterIndex * particles.size() / subCluster;
}

int parallelPso::subClusterEndPos(int subClusterIndex)
{
    return (subClusterIndex + 1) * particles.size() / subCluster - 1;
}

void parallelPso::replaceValueInSubCluster(int subClusterIndex, Data &x, double &y)
{
    // find worst value
    int minIndex = 0;
    double minValue = -1e+100;
    for (int pos = subClusterStartPos(subClusterIndex); pos <= subClusterEndPos(subClusterIndex); pos++)
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
        particles[minIndex] = x;
    }
}

void parallelPso::propagateSubClusterValues()
{
    if (parallelPropagateMethod == "1to1")
    {
        int subCluster1 = rand() % subCluster;
        int subCluster2 = rand() % subCluster;
        if (subCluster1 == subCluster2)
            return;
        Data xx = particles[bestSubClusterIndex[subCluster1]];
        double yy = bestSubClusterValues[subCluster1];
        replaceValueInSubCluster(subCluster2, xx, yy);
    }
    else if (parallelPropagateMethod == "1toN")
    {
        int subCluster1 = rand() % subCluster;
        Data xx = particles[bestSubClusterIndex[subCluster1]];
        double yy = bestSubClusterValues[subCluster1];
        for (int i = 0; i < subCluster; i++)
        {
            if (i == subCluster1)
                continue;
            replaceValueInSubCluster(i, xx, yy);
        }
    }
    else if (parallelPropagateMethod == "Nto1")
    {
        int subCluster2 = rand() % subCluster;
        for (int i = 0; i < subCluster; i++)
        {
            if (i == subCluster2)
                continue;
            int subCluster1 = i;
            Data xx = particles[bestSubClusterIndex[subCluster1]];
            double yy = bestSubClusterValues[subCluster1];
            replaceValueInSubCluster(subCluster2, xx, yy);
        }
    }
    else if (parallelPropagateMethod == "NtoN")
    {
        for (int i = 0; i < subCluster; i++)
        {
            for (int j = 0; j < subCluster; j++)
            {
                if (i == j)
                    continue;
                int subCluster1 = i;
                int subCluster2 = j;
                Data xx = particles[bestSubClusterIndex[subCluster1]];
                double yy = bestSubClusterValues[subCluster1];
                replaceValueInSubCluster(subCluster2, xx, yy);
            }
        }
    }
}

bool parallelPso::checkGradientCriterion(Data &x)
{
    double dmin = 1e+100;
    int imin = 0;
    if (minimax.size() > 0)
    {
        dmin = getDistance(minimax[0], x);
        for (int j = 0; j < minimax.size(); j++)
        {
            double d = getDistance(minimax[j], x);
            if (d < dmin)
            {
                imin = j;
                dmin = d;
            }
        }
    }
    else
        return false;
    if (dmin < 1e-6 || (dmin < RC / localSearchCount && myProblem->getGradientCriterion(x, minimax[imin])))
        return true;
    return false;
}

bool parallelPso::terminated()
{
    int c = 0;
    for (int i = 0; i < subCluster; i++)
        if (this->checkSubCluster(i))
            c++;
    return generation >= parallelPsoGenerations || c >= subClusterEnable;
}

bool parallelPso::checkSubCluster(int subClusterName)
{
    //---------------------SIMILARITY CASE 1 QUANTITIES------------------------------

        double difference = fabs(newSum.at(subClusterName) - sum.at(subClusterName));
        //printf("%d] neWsum.at(%d) : %f Sum.at(%d) : %f different  : %lf\n", generation, subClusterName, newSum.at(subClusterName), subClusterName, sum.at(subClusterName), subClusterName, difference );
        sum.at(subClusterName) = newSum.at(subClusterName);

        if (difference < 1e-10)
            similarityCurrentCount.at(subClusterName)++;
        else
            similarityCurrentCount.at(subClusterName) = 0;
        if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
            return true;
    //---------------------SIMILARITY CASE 2 AVERAGE TERMS----------------------------
/*
    double difference = fabs(newMO.at(subClusterName) - MO.at(subClusterName));
    // printf("%d] newMO.at(%d) : %f MO.at(%d) : %f different  : %lf\n", generation, subClusterName, newMO.at(subClusterName), subClusterName, MO.at(subClusterName), subClusterName, difference );
    MO.at(subClusterName) = newMO.at(subClusterName);

    if (difference < 1e-10)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
        return true;
    //--------------------------------------------------------------------------------
    */
    return false;
}

void parallelPso::step()
{

    ++generation;
    Data distances;
    QRandomGenerator gen1 = QRandomGenerator();
    QRandomGenerator gen2 = QRandomGenerator();
    QRandomGenerator gen3 = QRandomGenerator();
    QRandomGenerator gen4 = QRandomGenerator();
    int t, i, j;
    double R, inertia;
    double r1, r2, tj, part1, part2, part3, trialf, rr;
    Data oldg, dg;
    bool found;
#pragma omp parallel for default(shared) private(t) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {
        if (this->checkSubCluster(t))
                continue;
#pragma omp parallel for default(shared) private(gen1, gen2, gen3, t, i, j, R, tj, part1, part2, part3, trialf, distances, rr, dg, found, oldg) num_threads(subCluster)
        for (i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {
            // printf("subClusterStartPos(%d)\tsubClusterEndPos(%d)\n",subClusterStartPos(t),subClusterEndPos(t-1));
            R = gen1.bounded(1.0);
            inertia = 0.5 + (R / 2.0);
            oldg = particles[i];
            for (j = 0; j < dimension; j++)
            {
                r1 = gen2.bounded(1.0);
                r2 = gen3.bounded(1.0);

                tj = velocitys[i][j];
                part1 = inertia * velocitys[i][j];
                part2 = parallelPsoC1 * r1 * (bestParticle[i][j] - particles[i][j]);
                part3 = parallelPsoC2 * r2 * (bestParticleInCluster[t][j] - particles[i][j]);
                velocitys[i][j] = part1 + part2 + part3;
                trialf = particles[i][j] + velocitys[i][j];
                if (trialf < lmargin[j] || trialf > rmargin[j])
                {
                    velocitys[i][j] = tj;
                    continue;
                }
                else
                {
                    particles[i][j] = trialf;
                }
            }
            distances.push_back(getDistance(particles[i], oldg));
            if (distances[i] > 1e-6)
            {
                if (psoLocalSearchRate > 0.0 && rr <= psoLocalSearchRate && !checkGradientCriterion(particles[i]))
                {
                    rr = gen4.bounded(1.0);
                    dg = particles[i];
                    fitness_array[i] = localSearch(particles[i]);
                    RC += getDistance(dg, particles[i]);
                    localSearchCount++;
                    found = false;
                    for (int j = 0; j < minimax.size(); j++)
                    {
                        if (getDistance(minimax[j], particles[i]) < 1e-5)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        minimax.push_back(particles[i]);
                    }
                }
                else
                    fitness_array[i] = fitness(particles[i]);
            }
        }
    }

#pragma omp parallel for default(shared) private(t) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {
        if (this->checkSubCluster(t))
                continue;
        bestSubClusterIndex[t] = 0;
        bestSubClusterValues[t] = 1e+100;
#pragma omp parallel for default(shared) private(i, t) num_threads(subCluster)
        for (i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {

            if (fitness_array[i] < bestFitness_array[i])
            {
                bestFitness_array[i] = fitness_array[i];
                bestParticle[i] = particles[i];
            }
            if (besty > bestFitness_array[i])
            {
                bestx = bestParticle[i];
                besty = bestFitness_array[i];
                bestF2xInCluster.at(t) = bestFitness_array[i];
                bestParticleInCluster.at(t) = bestParticle[i];
            }
            if (fitness_array[i] < bestSubClusterValues[t])
            {
                bestSubClusterIndex[t] = i;
                bestSubClusterValues[t] = fitness_array[i];
            }
        }

    }
    for (int j = 0; j < subCluster; j++)
    {
        //printf("subClusterStartPos(%d)\tsubClusterEndPos(%d)",subClusterStartPos(j),subClusterEndPos(j));
        newSum.at(j) = accumulate(&fitness_array.at(this->subClusterStartPos(j)), &fitness_array.at(this->subClusterEndPos(j)), 0);
        //printf("newSum.at(%d) = %f \n",j,newSum.at(j));
    }
    for (int j = 0; j < subCluster; j++)
    {
        newMO.at(j) = (double)newSum.at(j) / subCluster;
    }
    if (generation % pardePropagateRate && subCluster > 1 && subClusterEnable > 1)
        propagateSubClusterValues();

}

void parallelPso::init()
{

    subCluster = params["subCluster"].toString().toInt();
    subClusterEnable = params["subClusterEnable"].toString().toInt();
    parallelPsoParticles = params["parallelPsoParticles"].toString().toInt();
    parallelPsoGenerations = params["parallelPsoGenerations"].toString().toInt();
    parallelPsoC1 = params["parallelPsoC1"].toString().toDouble();
    parallelPsoC2 = params["parallelPsoC2"].toString().toDouble();
    psoLocalSearchRate = params["psoLocalSearchRate"].toString().toDouble();
    parallelPropagateMethod = params["parallelPropagateMethod"].toString();
    similarityMaxCount = params["similarityMaxCount"].toString().toInt();
    pardePropagateRate = params["pardePropagateRate"].toString().toInt();
    dimension = myProblem->getDimension();
    bestParticle.resize(parallelPsoParticles * subCluster);
    velocitys.resize(parallelPsoParticles * subCluster);
    bestx.resize(dimension);
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    particles.resize(parallelPsoParticles * subCluster);
    fitness_array.resize(parallelPsoParticles * subCluster);
    bestFitness_array.resize(parallelPsoParticles * subCluster);
    for (int i = 0; i < parallelPsoParticles * subCluster; i++)
    {
        particles[i] = myProblem->getRandomPoint();
        fitness_array[i] = myProblem->funmin(particles[i]);
    }
    bestSubClusterIndex.resize(subCluster);
    bestSubClusterValues.resize(subCluster);
    similarityBestValue.resize(subCluster);
    similarityCurrentCount.resize(subCluster);
    generation = 0;
    besty = 1e+100;
    oldbesty = 1e+100;
    x1 = 0.0;
    x2 = 0.0;
    stopat = 0.0;
    variance = 0.0;
    RC = 0.0;
    localSearchCount = 0;
    minimax.clear();
    for (int i = 0; i < parallelPsoParticles * subCluster; i++)
    {
        bestParticle[i].resize(dimension);
        velocitys[i].resize(dimension);
        bestParticle[i] = particles[i];
        bestFitness_array[i] = fitness_array[i];

        for (int j = 0; j < dimension; j++)
        {
            double left = -(rmargin[j] - lmargin[j]) / 20.0;
            double right = (rmargin[j] - lmargin[j]) / 20.0;
            velocitys[i][j] = left + myProblem->randomDouble() * (right - left);
        }
    }
    similarityBestValue.resize(subCluster);
    similarityCurrentCount.resize(subCluster);
    sum.resize(subCluster);
    newSum.resize(subCluster);
    MO.resize(subCluster);
    newMO.resize(subCluster);
    bestParticleInCluster.resize(subCluster);
    bestF2xInCluster.resize(subCluster);
    for (int i = 0; i < subCluster; i++)
    {
        bestParticleInCluster.at(i).resize(dimension);
        bestF2xInCluster.at(i) = 1e+100;
        similarityBestValue.at(i) = 1e+100;
        similarityCurrentCount.at(i) = 0;
        sum.at(i) = 0;
        MO.at(i) = 0;
    }
}

void parallelPso::done()
{
    besty = localSearch(bestx);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
}

double parallelPso::fitness(Data &x)
{
    if (!myProblem->isPointIn(x))
        return 1e+100;
    return myProblem->funmin(x);
}

void parallelPso::Solve()
{
    Optimizer::Solve();
}

parallelPso::~parallelPso()
{
}

extern "C" PARALLELPSO_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new parallelPso(p);
}

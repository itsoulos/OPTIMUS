#include "parallelPso.h"

parallelPso::parallelPso(Problem *p) : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("parallelPsoParticles", "100", "Number of pso particles");
    addParameter("parallelPsoGenerations", "200", "Maximum number of pso generations");
    addParameter("parallelPsoC1", "0.5", "Pso c1 parameter");
    addParameter("parallelPsoC2", "0.5", "Pso c2 parameter");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("pardePropagateRate", "1", "The number of generations before the propagation takes place");
    addParameter("parallelPropagateMethod", "1to1", "The propagation method used. Available values: 1to1,1toN,Nto1,NtoN");
    addParameter("subCluster", "5", "number of subclusters for pso");
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
    return (subClusterIndex + 1) * particles.size() / subCluster -1;
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
        Data xx = bestParticleInCluster.at(subCluster1);
        double yy = bestF2xInCluster.at(subCluster1);
        replaceValueInSubCluster(subCluster2, xx, yy);
    }
    else if (parallelPropagateMethod == "1toN")
    {
        int subCluster1 = rand() % subCluster;
        Data xx = bestParticleInCluster.at(subCluster1);
        double yy = bestF2xInCluster.at(subCluster1);
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
            Data xx = bestParticleInCluster.at(subCluster1);
            double yy = bestF2xInCluster.at(subCluster1);
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
                Data xx = bestParticleInCluster.at(subCluster1);;
                double yy = bestF2xInCluster.at(subCluster1);
                replaceValueInSubCluster(subCluster2, xx, yy);
            }
        }
    }
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
    double bestValue;
    int bestIndex;
/*
    getBestValue(subClusterName, bestIndex, bestValue);
    //---------------------SIMILARITY CASE 1 QUANTITIES------------------------------

    double difference = fabs(newSum.at(subClusterName) - sum.at(subClusterName));
    // printf("%d] neWsum.at(%d) : %f Sum.at(%d) : %f different  : %lf\n", generation, subClusterName, newSum.at(subClusterName), subClusterName, sum.at(subClusterName), subClusterName, difference );
    sum.at(subClusterName) = newSum.at(subClusterName);

    if (difference < 1e-8)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
        return true;
    //---------------------SIMILARITY CASE 2 AVERAGE TERMS----------------------------

    double difference = fabs(newMO.at(subClusterName) - MO.at(subClusterName));
    // printf("%d] newMO.at(%d) : %f MO.at(%d) : %f different  : %lf\n", generation, subClusterName, newMO.at(subClusterName), subClusterName, MO.at(subClusterName), subClusterName, difference );
    MO.at(subClusterName) = newMO.at(subClusterName);

    if (difference < 1e-8)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
        return true;
    //--------------------------------------------------------------------------------
*/
    //----------------SIMILARITY CASE 3 DIFFERANCE F2X WITH F2X OLD-------------------

    double difference = fabs(bestF2xInClusterOLD.at(subClusterName) - bestF2xInCluster.at(subClusterName));
    printf("%d] F2x(%d) : %f F2xOLD.at(%d) : %f different  : %f\n", generation, subClusterName, bestF2xInCluster.at(subClusterName), subClusterName, bestF2xInClusterOLD.at(subClusterName), subClusterName, difference);

    if (difference < 1e-8)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
        return true;
/*
    doublebox_xx1.at(subClusterName) += bestValue;
    doublebox_xx2.at(subClusterName) += bestValue * bestValue;
    doublebox_variance.at(subClusterName) = doublebox_xx2.at(subClusterName) / generation - (doublebox_xx1.at(subClusterName) / generation) * (doublebox_xx1.at(subClusterName) / generation);
    if (bestValue < doublebox_best_value.at(subClusterName))
    {
        doublebox_best_value.at(subClusterName) = bestValue;
        doublebox_stopat.at(subClusterName) = doublebox_variance.at(subClusterName) / 2.0;
    }
    printf("%4d] doublebox_variance.at(%d) : %f doublebox_stopat.at(%d) : %f different  : %lf\n", generation, subClusterName, doublebox_variance.at(subClusterName), subClusterName, doublebox_stopat.at(subClusterName), subClusterName, fabs(doublebox_variance.at(subClusterName) - doublebox_stopat.at(subClusterName)));
    return doublebox_variance.at(subClusterName) <= doublebox_stopat.at(subClusterName);
*/
    return false;
}

void parallelPso::step()
{

    ++generation;
    int t, i, j;
#pragma omp parallel for private (t, i, j) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {
        bestF2xInClusterOLD.at(t) = bestF2xInCluster.at(t);
//#pragma omp parallel for  private (t, i, j) num_threads(subCluster)
        for (i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {
            QRandomGenerator gen1 = QRandomGenerator();
            QRandomGenerator gen2 = QRandomGenerator();
            QRandomGenerator gen3 = QRandomGenerator();
            double R, inertia, r1, r2, tj, part1, part2, part3, trialf;
            bool flag = false;
            // printf("subClusterStartPos(%d)\tsubClusterEndPos(%d)\n",subClusterStartPos(t),subClusterEndPos(t-1));
            R = gen1.bounded(1.0);
            inertia = 0.5 + (R / 2.0);

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
                    flag = true;
                }
            }
            if (flag)
                fitness_array[i] = myProblem->funmin(particles[i]);
#pragma omp critical
            {
                if (besty > fitness_array[i])
                {
                    besty = fitness_array[i];
                    bestx = particles[i];
                    bestParticle[i] = particles[i];

                }
                if (bestF2xInCluster.at(t) > fitness_array[i] )
                {
                    bestF2xInCluster.at(t) = fitness_array[i];
                    bestParticleInCluster.at(t) = particles[i];
                }

            }
        }
    }

    for (int k = 0; k < subCluster; k++)
        newSum.at(k) = accumulate(&fitness_array.at(this->subClusterStartPos(k)), &fitness_array.at(this->subClusterEndPos(k)), 0);

    for (int k = 0; k < subCluster; k++)
    {
        newMO.at(k) = (double)newSum.at(k) / subCluster;
    }

    if (generation % pardePropagateRate)
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
    for (int i = 0; i < parallelPsoParticles * subCluster; i++)
    {
        particles[i] = myProblem->getRandomPoint();
        fitness_array[i] = myProblem->funmin(particles[i]);
    }
    similarityBestValue.resize(subCluster);
    similarityCurrentCount.resize(subCluster);
    generation = 0;
    besty = 1e+100;

    for (int i = 0; i < parallelPsoParticles * subCluster; i++)
    {
        bestParticle[i].resize(dimension);
        velocitys[i].resize(dimension);
        bestParticle[i] = particles[i];

        for (int j = 0; j < dimension; j++)
        {
            double left = -(rmargin[j] - lmargin[j]) / 20.0;
            double right = (rmargin[j] - lmargin[j]) / 20.0;
            //velocitys[i][j] = left + myProblem->randomDouble() * (right - left);
            velocitys[i][j] = 0;
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
    bestF2xInClusterOLD.resize(subCluster);
    for (int i = 0; i < subCluster; i++)
    {
        bestParticleInCluster.at(i).resize(dimension);
        bestF2xInCluster.at(i) = 1e+100;
        similarityBestValue.at(i) = 1e+100;
        similarityCurrentCount.at(i) = 0;
        sum.at(i) = 0;
        MO.at(i) = 0;
        for (int j = subClusterStartPos(i); j <= subClusterEndPos(i); j++)
        {
            if (bestF2xInCluster.at(i) > fitness_array[j] )
            {
                bestF2xInCluster.at(i) = fitness_array[j];
                bestParticleInCluster.at(i) = particles[j];
            }
        }

    }

    doublebox_xx1.resize(subCluster);
    doublebox_xx2.resize(subCluster);
    doublebox_best_value.resize(subCluster);
    doublebox_stopat.resize(subCluster);
    doublebox_variance.resize(subCluster);
    for (int i = 0; i < subCluster; i++)
    {
        doublebox_xx1.at(i) = 0.0;
        doublebox_xx2.at(i) = 0.0;
        doublebox_best_value.at(i) = 1e+100;
        doublebox_stopat.at(i) = 1e+100;
        doublebox_variance.at(i) = 1e+100;
    }
    omp_set_dynamic(0);
    omp_set_num_threads(subCluster);
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

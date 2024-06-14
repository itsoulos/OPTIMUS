#include "PMNEW.h"

PMNEW::PMNEW(Problem *p) : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("optimization", "4", "Optimization Method [1=>DE, 2=>PSO, 3=>MULT, 4=>DEPSO, 5=>DEMULT, 6=>PSOMULT, 7=>DEPSOMULT]");
    addParameter("populationofcluster", "50", "Number of population of subclusters");
    addParameter("subClusters", "5", "Number of subclusters");
    addParameter("subClustersEnable", "1", "Number of subclusters for terminated");
    addParameter("propagation", "0", "Propagation [0] or [1]");
    addParameter("maxiterations", "100", "Maximum iterations");
    addParameter("stoppingrule", "best", "Termination criterion (mean,best,doublebox,ali)");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("differentialWeight", "0.8", "differentialWeight");
    addParameter("crossoverProbability", "0.9", "crossoverProbability");
    addParameter("c1", "0.5", "Pso c1 parameter");
    addParameter("c2", "0.5", "Pso c2 parameter");
    addParameter("localRate", "0.005", "Local search rate");
}
bool PMNEW::checkSubCluster(int subCluster)
{

    if (stoppingrule == "mean" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double difference1 = fabs(oldSum.at(subCluster) - sum.at(subCluster));
            //printf("sum=%f oldSum=%f different=%f\n", sum.at(subCluster), oldSum.at(subCluster), difference1);
            oldSum.at(subCluster) = sum.at(subCluster);
            if (difference1 < 1e-6)
                similarityCurrentCountMean.at(subCluster)++;
            else
                similarityCurrentCountMean.at(subCluster) = 0;

            if (similarityCurrentCountMean.at(subCluster) > similarityMaxCount)
                return true;
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double difference2 = fabs(oldSum.at(subCluster) - sum.at(subCluster));
            //printf("sum=%f oldSum=%f different=%f\n", sum.at(subCluster), oldSum.at(subCluster), difference2);
            oldSum.at(subCluster) = sum.at(subCluster);
            if (difference2 < 1e-6)
                similarityCurrentCountMean.at(subCluster)++;
            else
                similarityCurrentCountMean.at(subCluster) = 0;
            if (similarityCurrentCountMean.at(subCluster) > similarityMaxCount)
                return true;
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double difference3 = fabs(oldSum.at(subCluster) - sum.at(subCluster));
            oldSum.at(subCluster) = sum.at(subCluster);
            if (difference3 < 1e-6)
                similarityCurrentCountMean.at(subCluster)++;
            else
                similarityCurrentCountMean.at(subCluster) = 0;
            if (similarityCurrentCountMean.at(subCluster) > similarityMaxCount)
                return true;
        }
    }
    if (stoppingrule == "best" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double difference4 = fabs((besty_old.at(subCluster)) - besty.at(subCluster));
            //printf("besty=%f besty_old=%f different=%f\n", besty.at(subCluster), besty_old.at(subCluster), difference4);
            besty_old.at(subCluster) = besty.at(subCluster);
            if (difference4 < 1e-6)
                similarityCurrentCountBest.at(subCluster)++;
            else
                similarityCurrentCountBest.at(subCluster) = 0;
            if (similarityCurrentCountBest.at(subCluster) > similarityMaxCount)
                return true;
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double difference5 = fabs((besty_old.at(subCluster)) - besty.at(subCluster));
            besty_old.at(subCluster) = besty.at(subCluster);
            //printf("besty=%f besty_old=%f different=%f\n", besty.at(subCluster), besty_old.at(subCluster), difference5);
            if (difference5 < 1e-6)
                similarityCurrentCountBest.at(subCluster)++;
            else
                similarityCurrentCountBest.at(subCluster) = 0;
            if (similarityCurrentCountBest.at(subCluster) > similarityMaxCount)
                return true;
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double difference6 = fabs((besty_old.at(subCluster)) - besty.at(subCluster));
            besty_old.at(subCluster) = besty.at(subCluster);
            if (difference6 < 1e-6)
                similarityCurrentCountBest.at(subCluster)++;
            else
                similarityCurrentCountBest.at(subCluster) = 0;
            if (similarityCurrentCountBest.at(subCluster) > similarityMaxCount)
                return true;
        }
    }
    if (stoppingrule == "doublebox" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double fmin1 = fabs(1.0 + fabs(besty.at(subCluster)));
            if (iter <= 1)
            {
                x1.at(subCluster) = 0.0;
                x2.at(subCluster) = 0.0;
            }
            x1.at(subCluster) += fmin1;
            x2.at(subCluster) += fmin1 * fmin1;
            variance.at(subCluster) = x2.at(subCluster) / (iter + 1) - (x1.at(subCluster) / (iter + 1)) * (x1.at(subCluster) / (iter + 1));
            variance.at(subCluster) = fabs(variance.at(subCluster));

            if(iter ==0 || besty.at(subCluster) < yDB.at(subCluster) )
            {
                yDB.at(subCluster) = besty.at(subCluster);
                stopat.at(subCluster) = variance.at(subCluster) / 2.0;
            }
            if (variance.at(subCluster) <= stopat.at(subCluster)  && iter > similarityMaxCount)
                return true;
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double fmin2 = fabs(1.0 + fabs(besty.at(subCluster)));
            if (iter <= 1)
            {
                x1.at(subCluster) = 0.0;
                x2.at(subCluster) = 0.0;
            }
            x1.at(subCluster) += fmin2;
            x2.at(subCluster) += fmin2 * fmin2;
            variance.at(subCluster) = x2.at(subCluster) / (iter + 1) - (x1.at(subCluster) / (iter + 1)) * (x1.at(subCluster) / (iter + 1));
            variance.at(subCluster) = fabs(variance.at(subCluster));

            if(iter ==0 || besty.at(subCluster) < yDB.at(subCluster) )
            {
                yDB.at(subCluster) = besty.at(subCluster);
                stopat.at(subCluster) = variance.at(subCluster) / 2.0;
            }
            if (variance.at(subCluster) <= stopat.at(subCluster)  && iter > similarityMaxCount)
                return true;
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double fmin3 = fabs(1.0 + fabs(besty.at(subCluster)));
            if (iter <= 1)
            {
                x1.at(subCluster) = 0.0;
                x2.at(subCluster) = 0.0;
            }
            x1.at(subCluster) += fmin3;
            x2.at(subCluster) += fmin3 * fmin3;
            variance.at(subCluster) = x2.at(subCluster) / (iter + 1) - (x1.at(subCluster) / (iter + 1)) * (x1.at(subCluster) / (iter + 1));
            variance.at(subCluster) = fabs(variance.at(subCluster));

            if(iter ==0 || besty.at(subCluster) < yDB.at(subCluster) )
            {
                yDB.at(subCluster) = besty.at(subCluster);
                stopat.at(subCluster) = variance.at(subCluster) / 2.0;
            }
            if (variance.at(subCluster) <= stopat.at(subCluster)  && iter > similarityMaxCount)
                return true;
        }
    }

    return iter >= maxiterations;
}
bool PMNEW::terminated()
{

    int c = 0;
    for (int i = 0; i < subClusters; i++)
    {
        if (this->checkSubCluster(i))            
        {
            subCEnable.at(i) = 1;
            c++;
        }
    }
    //printf("c=%d  subClustersEnable=%d\n",c, subClustersEnable);
    //return (iter >= maxiterations) || c>=accumulate(subCEnable.begin(),subCEnable.end(),0);
    return (iter >= maxiterations) || c>=subClustersEnable;
}

double PMNEW::randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

void PMNEW::updateWithArmadillo(int subcluster)
{

}

void PMNEW::updateWithDE(int subCluster)
{

    int pos = 0;
    Data newX;
    double newMin;
    int it;
    for (it = 0; it < populationofcluster; it++)
    {
        int a = it;
        int b = it;
        int c = it;

        while (a == it || b == it || c == it || a == b || a == c || b == c)
        {
            a = rand() % populationofcluster;
            b = rand() % populationofcluster;
            c = rand() % populationofcluster;
            if (a < 0)
                a = it;
            if (b < 0)
                b = it;
            if (c < 0)
                c = it;
        }
        Data z;
        z.resize(dimension);
        differentialWeight = 0.5 + (randMToN(0.0, 1.0)) / 2;

        for (int j = 0; j < dimension; j++)
        {
            z[j] = elements.at(subCluster).at(a).at(j) + differentialWeight * (elements.at(subCluster).at(b).at(j) - elements.at(subCluster).at(c).at(j));
        }
        int R = rand() % dimension;

        Data r;
        r.resize(dimension);
        for (int i = 0; i < dimension; i++)
            r[i] = randMToN(0.0, 1.0);

        newX.resize(dimension);
        bool flag = false;
        for (int j = 0; j < dimension; j++)
        {
            if (r[j] < crossoverProbability || j == R && (z[j] >= lmargin[j] && z[j] <= rmargin[j]))
            {
                newX[j] = z[j];
                flag = true;
            }
            else
                newX[j] = elements.at(subCluster).at(it).at(j);
        }

        if (flag)
        {
            newMin = myProblem->funmin(newX);
            if (localRate > 0 && myProblem->randomDouble() <= localRate)
            {
                fitness_array.at(subCluster).at(it) = localSearch(elements.at(subCluster).at(it));
            }
        }

#pragma omp critical
        {
            if (newMin < fitness_array.at(subCluster).at(it))
            {
                elements.at(subCluster).at(it) = newX;
                fitness_array.at(subCluster).at(it) = newMin;
            }

            if (newMin < besty.at(subCluster))
            {
                elements.at(subCluster).at(it) = newX;
                besty.at(subCluster) = newMin;
                bestx.at(subCluster) = elements.at(subCluster).at(it);
                pos = it;
            }
            if (bestyAll > besty.at(subCluster))
            {
                bestyAll = besty.at(subCluster);
                bestxAll = elements.at(subCluster).at(it);
            }
        }

    }
}
void PMNEW::updateWithPso(int subCluster)
{

    int pos;
    double inertia, R, newFx;
    for (int i = 0; i < populationofcluster; i++)
    {

        bool flag = false;
        Data oldelement = elements.at(subCluster).at(i);
        for (int j = 0; j < dimension; j++)
        {
            R = randMToN(0.0, 1.0);
            inertia = 0.5 + (R / 2.0);
            double r1 = randMToN(0.0, 1.0);
            double r2 = randMToN(0.0, 1.0);
            double tj = velocitys.at(subCluster).at(i).at(j);

            double part1 = inertia * velocitys.at(subCluster).at(i).at(j);
            double part2 = c1 * r1 * (bestPoints_PSO.at(subCluster).at(i).at(j) - elements.at(subCluster).at(i).at(j));
            double part3 = c2 * r2 * (bestxPSO.at(subCluster).at(j) - elements.at(subCluster).at(i).at(j));
            velocitys.at(subCluster).at(i).at(j) = part1 + part2 + part3;

            double trialf = elements.at(subCluster).at(i).at(j) + velocitys.at(subCluster).at(i).at(j);

            if (trialf < lmargin[j] || trialf > rmargin[j])
            {
                velocitys.at(subCluster).at(i).at(j) = tj;
                continue;
            }
            else
            {
                elements.at(subCluster).at(i).at(j) = trialf;
                flag = true;
            }
        }

        if (flag)
        {
            // fitness_array.at(subCluster).at(i) = myProblem->funmin(elements.at(subCluster).at(i));
            newFx = myProblem->funmin(elements.at(subCluster).at(i));
            if (localRate > 0 && myProblem->randomDouble() <= localRate)
            {
                fitness_array.at(subCluster).at(i) = localSearch(elements.at(subCluster).at(i));
            }
        }
    }

    for (int i = 0; i < populationofcluster; i++)
    {
#pragma omp critical
        {

            if (newFx < fitness_array.at(subCluster).at(i))
            {
                fitness_array.at(subCluster).at(i) = newFx;
                bestPoints_PSO.at(subCluster).at(i) = elements.at(subCluster).at(i);
                pos = i;
            }

            if (bestyPSO.at(subCluster) > newFx)
            {
                bestxPSO.at(subCluster) = elements.at(subCluster).at(i);
                bestyPSO.at(subCluster) = newFx;
            }
            if (besty.at(subCluster) > fitness_array.at(subCluster).at(i))
            {
                bestx.at(subCluster) = elements.at(subCluster).at(i);
                besty.at(subCluster) = fitness_array.at(subCluster).at(i);
            }
            if (bestyAll > besty.at(subCluster))
            {
                bestyAll = besty.at(subCluster);
                bestxAll = elements.at(subCluster).at(i);
            }
        }
    }

}

void PMNEW::updateWithMultistart(int subCluster)
{
    besty_old.at(subCluster) = besty.at(subCluster);
    int pos;
    for (int i = 0; i < populationofcluster; i++)
    {
        fitness_array.at(subCluster).at(i) = localSearch(elements.at(subCluster).at(i));
#pragma omp critical
        {
            if (besty.at(subCluster) > fitness_array.at(subCluster).at(i))
            {
                bestx.at(subCluster) = elements.at(subCluster).at(i);
                besty.at(subCluster) = fitness_array.at(subCluster).at(i);
                pos = i;
            }
            if (bestyAll > besty.at(subCluster))
            {
                bestyAll = besty.at(subCluster);
                bestxAll = elements.at(subCluster).at(i);
            }
        }
    }
}
void PMNEW::checkMean()
{
    for (int i = 0; i < subClusters; i++)
    {
        sum.at(i) = accumulate(fitness_array.at(i).begin(), fitness_array.at(i).end(), 0.0);
        sum.at(i) = sum.at(i) / populationofcluster;
    }
}
void PMNEW::step()
{
    ++iter;
    /*
     * 1=>DE
     * 2=>PSO
     * 3=>MULT
     * 4=>DE+PSO
     * 5=>DE+MULT
     * 6=>PSO+MULT
     * 7=>DE+PSO+MULT
     */

    int subCluster;
#pragma omp parallel for private(subCluster) schedule(static) num_threads(subClusters)
    for (subCluster = 0; subCluster < subClusters; subCluster++)
    {
        if (optimization == 1)
            updateWithDE(subCluster);
        else if (optimization == 2)
            updateWithPso(subCluster);
        else if (optimization == 3)
            updateWithMultistart(subCluster);
        else if (optimization == 4)
        {
            if (subCluster % 2 == 0)
                updateWithDE(subCluster);
            else
                updateWithPso(subCluster);
        }
        else if (optimization == 5)
        {
            if (subCluster % 2 == 0)
                updateWithDE(subCluster);
            else
                updateWithMultistart(subCluster);
        }
        else if (optimization == 6)
        {
            if (subCluster % 2 == 0)
                updateWithPso(subCluster);
            else
                updateWithMultistart(subCluster);
        }
        else if (optimization == 7)
        {
            if (subCluster % 3 == 0)
                updateWithDE(subCluster);
            else if (subCluster % 3 == 1)
                updateWithPso(subCluster);
            else
                updateWithMultistart(subCluster);
        }
        else if (optimization == 7)
        {
             updateWithArmadillo(subCluster);
        }
    }
    //if (iter % 5 ==0)
        if (propagation && subClusters > 1)
            propagate();
    checkMean();
}
void PMNEW::propagate()
{

    vector<double> max;
    max.resize(subClusters);
    vector<double> min;
    min.resize(subClusters);
    vector<int> posmax;
    posmax.resize(subClusters);
    vector<int> posmin;
    posmin.resize(subClusters);
    for (int i = 0; i < subClusters; i++)
    {
        max.at(i) = *max_element(fitness_array.at(i).begin(), fitness_array.at(i).end());
        posmax.at(i) = distance(fitness_array.at(i).begin(), max_element(fitness_array.at(i).begin(), fitness_array.at(i).end()));
        min.at(i) = *min_element(fitness_array.at(i).begin(), fitness_array.at(i).end());
        posmin.at(i) = distance(fitness_array.at(i).begin(), min_element(fitness_array.at(i).begin(), fitness_array.at(i).end()));
    }
    double bestmin = min.at(0);
    int pos = 0;
    for (int i = 0; i < subClusters; i++)
    {
        if (min.at(i) < bestmin)
        {
            bestmin = min.at(i);
            pos = i;
        }
    }
    for (int i = 0; i < subClusters; i++)
    {
        fitness_array.at(i).at(posmax.at(i)) = bestmin;
        elements.at(i).at(posmax.at(i)) = elements.at(pos).at(posmin.at(i));
    }
}

void PMNEW::init()
{
    maxiterations = params["maxiterations"].toString().toInt();

    localRate = params["localRate"].toString().toDouble();
    optimization = params["optimization"].toString().toInt();
    subClusters = params["subClusters"].toString().toInt();
    subClustersEnable = params["subClustersEnable"].toString().toInt();
    propagation = params["propagation"].toString().toInt();

    if (subClustersEnable > subClusters)
        subClustersEnable = subClusters;
    similarityMaxCount = params["similarityMaxCount"].toString().toInt();
    stoppingrule = params["stoppingrule"].toString();

    populationofcluster = params["populationofcluster"].toString().toInt();

    //printf("subClustersEnable:%d\nsubClusters:%d\npopulationofcluster:%d\npropagation:%d\n",subClustersEnable,subClusters,populationofcluster,propagation);

    population = subClusters * populationofcluster;
    c1 = params["c1"].toString().toDouble();
    c2 = params["c2"].toString().toDouble();
    crossoverProbability = params["crossoverProbability"].toString().toInt();
    differentialWeight = params["differentialWeight"].toString().toInt();
    stoppingrule = params["stoppingrule"].toString();
    dimension = myProblem->getDimension();

    elements.resize(subClusters);
    bestPoints_PSO.resize(subClusters);
    velocitys.resize(subClusters);
    fitness_array.resize(subClusters);
    bestxPSO.resize(subClusters);
    bestyPSO.resize(subClusters);
    bestx.resize(subClusters);
    bestxAll.resize(myProblem->getDimension());
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    iter = 0;
    besty.resize(subClusters);
    besty_old.resize(subClusters);

    similarityCurrentCountMean.resize(subClusters);
    similarityCurrentCountBest.resize(subClusters);
    x1.resize(subClusters);
    x2.resize(subClusters);
    stopat.resize(subClusters);
    variance.resize(subClusters);
    yDB.resize(subClusters);
    subCEnable.resize(subClusters);
    for (int i = 0; i < subClusters; i++)
    {
        subCEnable.at(i) = 0;
    }
    for (int i = 0; i < subClusters; i++)
    {

        similarityCurrentCountMean.at(i) = 0;
        similarityCurrentCountBest.at(i) = 0;
        x1.at(i) = 0;
        x2.at(i) = 0;
        stopat.at(i) = 0;
        variance.at(i) = 0;
        besty.at(i) = 1e+100;
        besty_old.at(i) = 1e+100;
        yDB.at(i) = 1e+100;

        elements.at(i).resize(populationofcluster);
        velocitys.at(i).resize(populationofcluster);
        fitness_array.at(i).resize(populationofcluster);
        bestPoints_PSO.at(i).resize(populationofcluster);
        bestxPSO.at(i).resize(myProblem->getDimension());
    }

    for (int i = 0; i < subClusters; i++)
    {
        for (int j = 0; j < populationofcluster; j++)
        {

            elements.at(i).at(j).resize(myProblem->getDimension());
            elements.at(i).at(j) = myProblem->getRandomPoint();
            bestPoints_PSO.at(i).at(j) = elements.at(i).at(j);
            fitness_array.at(i).at(j) = myProblem->funmin(elements.at(i).at(j));
            velocitys.at(i).at(j).resize(dimension);
            for (int k = 0; k < dimension; k++)
            {
                velocitys.at(i).at(j).at(k) = 0;
            }
        }
    }

    sum.resize(subClusters);
    oldSum.resize(subClusters);
    for (int i = 0; i < subClusters; i++)
    {
        sum.at(i) = accumulate(fitness_array.at(i).begin(), fitness_array.at(i).end(), 0.0);
        sum.at(i) = sum.at(i) / populationofcluster;
    }
}

void PMNEW::done()
{
    bestyAll = localSearch(bestxAll);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
}

double PMNEW::fitness(Data &x)
{
    if (!myProblem->isPointIn(x))
        return 1e+100;
    return myProblem->funmin(x);
}

void PMNEW::Solve()
{
    Optimizer::Solve();
}

PMNEW::~PMNEW()
{
}

extern "C" PMNEW_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new PMNEW(p);
}

#include "pdg.h"

pdg::pdg(Problem *p)
    : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("chromosomes", "100", "Number of chromosomes");
    addParameter("generations", "100", "Number of generations");
    addParameter("selectionRate", "0.10", "Selection rate");
    addParameter("mutationRate", "0.05", "Mutation rate");
    addParameter("localRate", "0.05", "Local search rate");
    addParameter("stopRule", "doubleBox", "Stopping rule (doubleBox,similarity,generations,stoponzero)");
    addParameter("debug", "no", "If it yes the full debug will be printed");
    addParameter("pNumber", "5", "the number of samples for propagation");
    addParameter("subClusterEnable", "1", "the number of subclusters that play a role in the termination rule: [1, islands]");
    addParameter("subCluster", "5", "number of subclusters for pDoubleGenetic");
    addParameter("propagateMethod", "1to1", "The propagation method used. Available values: 1to1,1toN,Nto1,NtoN");
    addParameter("propagateRate", "1", "The number of generations before the propagation takes place");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
}
bool sortByFirstDesc(const pair<double, int> &a, const pair<double, int> &b)
{
    return a.first < b.first;
}
bool sortByFirstDesc2(const pair<double, Data> &a, const pair<double, Data> &b)
{
    return a.first < b.first;
}
void pdg::replace(int subClusterIndex, vector<pair<double, Data>> chrom)
{
    vector<pair<double, int>> tmp;
    tmp.resize(populationOfCluster);
    for (int i = 0; i < populationOfCluster; i++)
    {
        tmp.at(i).first = -1e+100;
        tmp.at(i).second = 0;
    }
    for (int i = 0; i < populationOfCluster; i++)
    {
        if (tmp.at(i).first < fitnessArray.at(subClusterIndex).at(i))
        {
            tmp.at(i).first = fitnessArray.at(subClusterIndex).at(i);
            tmp.at(i).second = i;
        }
    }
    sort(tmp.begin(), tmp.end(), sortByFirstDesc);
    tmp.resize(pNumber);
    vector<int> tmp2;
    for (int i = 0; i < pNumber; i++)
    {
        tmp2.push_back(tmp.at(i).second);
    }
    for (int i = 0; i < populationOfCluster; i++)
    {
        if (find(tmp2.begin(), tmp2.end(), i) != tmp2.end())
        {
            fitnessArray.at(subClusterIndex).at(i) = chrom.back().first;
            chromosome.at(subClusterIndex).at(i) = chrom.back().second;
            chrom.pop_back();
        }
    }
}


void pdg::propagate()
{
    if (pNumber > 0 && subCluster > 1)
    {
        if (propagateMethod == "1to1")
        {
            int subCluster1, subCluster2;
            do
            {
                subCluster1 = rand() % subCluster;
                subCluster2 = rand() % subCluster;
            } while (subCluster1 == subCluster2);


            vector<pair<double, Data>> tmp;
            tmp.resize(populationOfCluster);

            for (int i = 0; i < populationOfCluster; i++)
                tmp.at(i).first = 1e+100;

            for (int i = 0; i < populationOfCluster; i++)
            {
                if (tmp.at(i).first > fitnessArray.at(subCluster1).at(i))
                {
                    tmp.at(i).first = fitnessArray.at(subCluster1).at(i);
                    tmp.at(i).second = chromosome.at(subCluster1).at(i);
                }
            }
            sort(tmp.begin(), tmp.end());
            //sort(tmp.begin(), tmp.end(), sortByFirstDesc);
            tmp.resize(pNumber);
            replace(subCluster2, tmp);
        }
        else if (propagateMethod == "1toN")
        {
            int subCluster1 = rand() % subCluster;
            vector<pair<double, Data>> tmp;

            for (int i = 0; i < subCluster; i++)
            {
                tmp.resize(populationOfCluster);
                for (int k = 0; k < populationOfCluster; k++)
                    tmp.at(k).first = 1e+100;
                if (i == subCluster1)
                    continue;

                for (int i = 0; i < populationOfCluster; i++)
                {
                    if (tmp.at(i).first > fitnessArray.at(subCluster1).at(i))
                    {
                        tmp.at(i).first = fitnessArray.at(subCluster1).at(i);
                        tmp.at(i).second = chromosome.at(subCluster1).at(i);
                    }
                }
                sort(tmp.begin(), tmp.end());
                tmp.resize(pNumber);
                replace(i, tmp);
            }
        }
        else if (propagateMethod == "Nto1")
        {
            int subCluster2 = rand() % subCluster;
            vector<pair<double, Data>> tmp;
            for (int e = 0; e < subCluster; e++)
            {
                tmp.resize(populationOfCluster);
                for (int k = 0; k < populationOfCluster; k++)
                    tmp.at(k).first = 1e+100;
                if (e == subCluster2)
                    continue;
                int subCluster1 = e;
                for (int i = 0; i < populationOfCluster; i++)
                {
                    if (tmp.at(i).first > populationOfCluster)
                    {
                        tmp.at(i).first = fitnessArray.at(subCluster1).at(i);
                        tmp.at(i).second = chromosome.at(subCluster1).at(i);
                    }
                }
                sort(tmp.begin(), tmp.end());
                tmp.resize(pNumber);
                replace(subCluster2, tmp);
            }
        }
        else if (propagateMethod == "NtoN")
        {
            vector<pair<double, Data>> tmp;
            for (int i = 0; i < subCluster; i++)
            {
                for (int j = 0; j < subCluster; j++)
                {
                    tmp.resize(populationOfCluster);
                    for (int k = 0; k < populationOfCluster; k++)
                        tmp.at(k).first = 1e+100;
                    if (i == j)
                        continue;
                    int subCluster1 = i;
                    int subCluster2 = j;

                    for (int k = 0; k < populationOfCluster; k++)
                    {
                        if (tmp.at(k).first > fitnessArray.at(subCluster1).at(i))
                        {
                            tmp.at(k).first = fitnessArray.at(subCluster1).at(i);
                            tmp.at(k).second = chromosome.at(subCluster1).at(i);
                        }
                    }
                    sort(tmp.begin(), tmp.end());
                    tmp.resize(pNumber);
                    replace(subCluster2, tmp);
                }
            }
        }
    }
}
void pdg::calcFitnessArray(int subClusterIndex)
{
    bestF2xInClusterOLD.at(subClusterIndex) = bestF2xInCluster.at(subClusterIndex);
    double dmin = 1e+100;


    for (int j = 0; j < populationOfCluster; j++)
    {
        fitnessArray.at(subClusterIndex).at(j) = myProblem->funmin(chromosome.at(subClusterIndex).at(j));
        if (localRate > 0 && myProblem->randomDouble() <= localRate)
        {
            fitnessArray.at(subClusterIndex).at(j) = localSearch(chromosome.at(subClusterIndex).at(j));
        }
        if (fitnessArray.at(subClusterIndex).at(j) < dmin)
        {
            dmin = fitnessArray.at(subClusterIndex).at(j);
            chrom =chromosome.at(subClusterIndex).at(j);
        }
        if (debug == "yes" && j % 10 == 0)
        {
            printf("%d:%20.12lg ", j, dmin);
            fflush(stdout);
        }
    }
    if (debug == "yes")
        printf("\n");
}

void pdg::select(int subClusterIndex)
{

    Data itemp;
    itemp.resize(myProblem->getDimension());

    for (int i = 0; i < populationOfCluster; i++)
    {
        for (int j = 0; j < populationOfCluster - 1; j++)
        {
            if (fitnessArray.at(subClusterIndex).at(j + 1) < fitnessArray.at(subClusterIndex).at(j))
            {
                itemp = chromosome.at(subClusterIndex).at(j);
                chromosome.at(subClusterIndex).at(j) = chromosome.at(subClusterIndex).at(j + 1);
                chromosome.at(subClusterIndex).at(j + 1) = itemp;
                double dtemp = fitnessArray.at(subClusterIndex).at(j);
                fitnessArray.at(subClusterIndex).at(j) = fitnessArray.at(subClusterIndex).at(j + 1);
                fitnessArray.at(subClusterIndex).at(j + 1) = dtemp;
            }
        }
    }
}
void pdg::doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2)
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
void pdg::crossover(int subClusterIndex)
{

    int nchildren = (int)((1.0 - selectionRate) * populationOfCluster);
    if (!(nchildren % 2 == 0))
        nchildren++;
    int populationOfCluster_children = 0;
    Data parent0, parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    while (populationOfCluster < nchildren )
    {
        tournament(subClusterIndex, parent0, parent1);

        doubleCrossover(parent0, parent1, children.at(subClusterIndex).at(populationOfCluster_children), children.at(subClusterIndex).at(populationOfCluster_children + 1));

        populationOfCluster_children += 2;

    }
    for (int i = 0; i < nchildren; i++)
    {
        chromosome.at(subClusterIndex).at(populationOfCluster - i - 1) = children.at(subClusterIndex).at(i);
    }

    generation.at(subClusterIndex)++;
    //printf("generation.at(%d)=%d\n", subClusterIndex, generation.at(subClusterIndex));
}

void pdg::mutate(int subClusterIndex)
{
    int istart = 1;

    for (int i = istart; i < populationOfCluster; i++)
    {
        for (int j = 0; j < myProblem->getDimension(); j++)
        {
            double r = myProblem->randomDouble();
            if (r <= mutationRate)
            {
                chromosome.at(subClusterIndex).at(i).at(j) = lmargin.at(j) + myProblem->randomDouble() * (rmargin.at(j) - lmargin.at(j));
            }
        }
    }

}

void pdg::getTournamentElement(int subClusterIndex, Data &x)
{

    const int tournament_size = (populationOfCluster <= 100) ? 4 : 10;
    double max_fitness = 1e+100;
    int max_index = -1;
    for (int j = 0; j < tournament_size; j++)
    {
        int r = randomChromosome();
        if (j == 0 || fitnessArray.at(subClusterIndex).at(r) < max_fitness)
        {
            max_index = r;
            max_fitness = fitnessArray.at(subClusterIndex).at(r);
        }
    }
    x = chromosome.at(subClusterIndex).at(max_index);
}

void pdg::tournament(int subClusterIndex, Data &p1, Data &p2)
{
    getTournamentElement(subClusterIndex, p1);
    getTournamentElement(subClusterIndex, p2);
}

int pdg::randomChromosome()
{
    double r = myProblem->randomDouble();

    int pos = r * populationOfCluster;
    if (pos == populationOfCluster)
        pos--;
    return pos;
}

bool pdg::terminated()
{
    //printf("subEnable=%d subClusterEnable=%d\n", subEnable, subClusterEnable);
    if ( subEnable >= subClusterEnable)
        return true;
    return false;
}
bool pdg::checkSubCluster(int subClusterName)
{


    if (stopRule == "similarity")
    {
        double difference = fabs(bestF2xInClusterOLD.at(subClusterName) - bestF2xInCluster.at(subClusterName));
        // printf("%d] F2x(%d) : %f F2xOLD.at(%d) : %f different  : %f\n", generation, subClusterName, bestF2xInCluster.at(subClusterName), subClusterName, bestF2xInClusterOLD.at(subClusterName), subClusterName, difference);
        //  if (bestF2xInClusterOLD.at(subClusterName) == bestF2xInCluster.at(subClusterName))
        if (difference < 1e-8)
            similarityCurrentCount.at(subClusterName)++;
        else
            similarityCurrentCount.at(subClusterName) = 0;
        if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount)
            return true;
        else
            return false;
    }else if (stopRule == "doubleBox")
    {

    // double bestValue = fabs(dmin);
    double bestValue = fabs(1.0 + bestF2xInCluster.at(subClusterName));
    doublebox_xx1.at(subClusterName) += bestValue;
    doublebox_xx2.at(subClusterName) += bestValue * bestValue;
    doublebox_variance.at(subClusterName) = doublebox_xx2.at(subClusterName) / (generation.at(subClusterName) + 1) - (doublebox_xx1.at(subClusterName) / (generation.at(subClusterName) + 1)) * (doublebox_xx1.at(subClusterName) / (generation.at(subClusterName) + 1));
    if (bestF2xInCluster.at(subClusterName) < bestF2xInClusterOLD.at(subClusterName) || generation.at(subClusterName) <= 1)
    {
        bestF2xInClusterOLD.at(subClusterName) = bestF2xInCluster.at(subClusterName);
        doublebox_stopat.at(subClusterName) = doublebox_variance.at(subClusterName) / 2.0;
    }
    if (doublebox_stopat.at(subClusterName) < 1e-5)
    {
        doublebox_stopat.at(subClusterName) = doublebox_variance.at(subClusterName) / 2.0;
    }

    // printf("%4d] doublebox_variance.at(%d) : %f doublebox_stopat.at(%d) : %f different  : %lf\n", generation, subClusterName, doublebox_variance.at(subClusterName), subClusterName, doublebox_stopat.at(subClusterName), subClusterName, fabs(doublebox_variance.at(subClusterName) - doublebox_stopat.at(subClusterName)));
    return generation.at(subClusterName) >= generations || (doublebox_variance.at(subClusterName) <= doublebox_stopat.at(subClusterName) && generation.at(subClusterName) >= 20);
}else
    {
        return generation.at(subClusterName) >= generations;
    }
    return false;
}

void pdg::step()
{
    subEnable = 0;
#pragma omp parallel for num_threads(subCluster)
    for (int i = 0; i < subCluster; i++)
    {
        if (checkSubCluster(i))
        {
            subEnable++;
            //terminated();
            continue;
        }

            if (generation.at(i))
                mutate(i);
            calcFitnessArray(i);
            select(i);
            crossover(i);
    }

    if (pNumber > 0)
    {
        propagate();
    }

}

void pdg::init()
{
    debug = params["debug"].toString();
    propagateRate = params["propagateRate"].toString().toInt();
    propagateMethod = params["propagateMethod"].toString();
    pNumber = params["pNumber"].toString().toInt();
    localRate = params["localRate"].toString().toDouble();
    populationOfCluster = params["chromosomes"].toString().toInt();
    generations = params["generations"].toString().toInt();
    stopRule = params["stopRule"].toString();
    selectionRate = params["selectionRate"].toString().toDouble();
    mutationRate = params["mutationRate"].toString().toDouble();
    subCluster = params["subCluster"].toString().toInt();
    subClusterEnable = params["subClusterEnable"].toString().toInt();
    similarityMaxCount = params["similarityMaxCount"].toString().toInt();

    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    chrom.resize(myProblem->getDimension());

    similarityCurrentCount.resize(subCluster);
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
    //if (subClusterEnable > subCluster)
    //    subClusterEnable = subCluster;

    chromosome.resize(subCluster);
    fitnessArray.resize(subCluster);
    children.resize(subCluster);
    generation.resize(subCluster);
    for (int i = 0; i < subCluster; i++)
    {
        chromosome.at(i).resize(populationOfCluster);
        fitnessArray.at(i).resize(populationOfCluster);
        children.at(i).resize(populationOfCluster);
    }
    for (int i = 0; i < subCluster; i++)
    {
        for (int j = 0; j < populationOfCluster; j++)
        {
            chromosome.at(i).at(j) = myProblem->getRandomPoint();
            fitnessArray.at(i).at(j) = myProblem->funmin(chromosome.at(i).at(j));
            children.at(i).at(j).resize(myProblem->getDimension());
        }
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
        generation.at(i) = 0;
        for (int j = 0; j < populationOfCluster; j++)
        {
            if (bestF2xInCluster.at(i) > fitnessArray.at(i).at(j))
            {
                bestF2xInCluster.at(i) = fitnessArray.at(i).at(j);
            }
        }
    }
    subEnable = 0;

}

void pdg::done()
{
    double c = localSearch(chrom);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
}

void pdg::Solve()
{
    Optimizer::Solve();
}

pdg::~pdg()
{
}

extern "C" PDG_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new pdg(p);
}

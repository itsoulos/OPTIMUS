#include "pdoublegenetic.h"

pDoubleGenetic::pDoubleGenetic(Problem *p)
    : Optimizer(p)
{
    before = std::chrono::system_clock::now();
    addParameter("double_chromosomes", "200", "Number of chromosomes");
    addParameter("double_generations", "100", "Number of generations");
    addParameter("double_selectionrate", "0.10", "Selection rate");
    addParameter("double_mutationrate", "0.05", "Mutation rate");
    addParameter("double_localsearchrate", "0.00", "Local search rate");
    addParameter("double_stoprule", "doublebox", "Stopping rule (doublebox,generations,stoponzero)");
    addParameter("double_debug", "no", "If it yes the full debug will be printed");
    addParameter("subClusterEnable", "1", "the number of subclusters that play a role in the termination rule: [1, islands]");
    addParameter("subCluster", "5", "number of subclusters for pDoubleGenetic");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("centers", "20", "centers");
}
double pDoubleGenetic::randMToN(double M, double N)
{
	double r = rand() *1.0/RAND_MAX;
	return M + r*(N-M);
}
int pDoubleGenetic::subClusterStartPos(int subClusterIndex)
{
    return subClusterIndex * double_chromosomes / subCluster;
}

int pDoubleGenetic::subClusterEndPos(int subClusterIndex)
{
    return (subClusterIndex + 1) * double_chromosomes / subCluster - 1;
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
    double r = randMToN(subClusterStartPos(subCluster), subClusterEndPos(subCluster));
    int pos = (int)r;//r * subClusterEndPos(subCluster);
    if (pos == subClusterEndPos(subCluster))
        pos--;
    return pos;
}

bool pDoubleGenetic::terminated()
{

    int c = 0;
    for (int i = 0; i < subCluster; i++)
        if (this->checkSubCluster(i))
            c++;
    return generation >= double_generations || c >= subClusterEnable;
    // return generation >= double_generations;
}
bool pDoubleGenetic::checkSubCluster(int subClusterName)
{

    double difference = fabs(bestF2xInClusterOLD.at(subClusterName) - bestF2xInCluster.at(subClusterName));
    //printf("%d] F2x(%d) : %f F2xOLD.at(%d) : %f different  : %f\n", generation, subClusterName, bestF2xInCluster.at(subClusterName), subClusterName, bestF2xInClusterOLD.at(subClusterName), subClusterName, difference);
    //if (bestF2xInClusterOLD.at(subClusterName) == bestF2xInCluster.at(subClusterName))
    if (difference < 1e-8)
        similarityCurrentCount.at(subClusterName)++;
    else
        similarityCurrentCount.at(subClusterName) = 0;
    if (similarityCurrentCount.at(subClusterName) >= similarityMaxCount && generation >= 20)
        return true;

    return false;
}
void pDoubleGenetic::step()
{

    if (generation)
    {
        int t, i;
        #pragma omp parallel for private(i, t) num_threads(subCluster)
        for (t = 0; t < subCluster; t++)
        {
            for (i = subClusterStartPos(t) + 1; i <= subClusterEndPos(t); i++)
            {

                for (int j = 0; j < myProblem->getDimension(); j++)
                {
                    double r = myProblem->randomDouble();
                    if (r <= mutation_rate)
                    {
                        chromosome[i][j] = lmargin[j] + myProblem->randomDouble() * (rmargin[j] - lmargin[j]);
                    }
                }
            }
        }
    }
    double rate = params["double_localsearchrate"].toString().toDouble();
    dmin = 1e+100;
    QString debug = params["double_debug"].toString();
    int t, i;
    #pragma omp parallel for private(i, t) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {

        bestF2xInClusterOLD.at(t) = bestF2xInCluster.at(t);

        for (i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {

            fitnessArray[i] = myProblem->funmin(chromosome[i]);
            // bestF2xInCluster.at(t) =  fitnessArray[i];

            if (rate > 0 && myProblem->randomDouble() <= rate)
            {
                fitnessArray[i] = localSearch(chromosome[i]);
            }
            if (bestF2xInCluster.at(t) > fitnessArray[i])
            {
                bestF2xInCluster.at(t) = fitnessArray[i];
                // bestParticleInCluster.at(t) = chromosome[i];
            }
#pragma omp critical
            {
                if (dmin > fitnessArray[i])
                {
                    dmin = fitnessArray[i];
                    bestx = chromosome[i];
                }
            }
            if (debug == "yes" && i % 10 == 0)
            {
                printf("%d:%20.12lg ", i, dmin);
                fflush(stdout);
            }
        }
    }
    if (debug == "yes")
        printf("\n");

    Data itemp;
    itemp.resize(myProblem->getDimension());

    int j;
    #pragma omp parallel for private(i, t, j) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {

        for (i = subClusterStartPos(t); i <= subClusterEndPos(t); i++)
        {
            for (j = subClusterStartPos(t); j <= subClusterEndPos(t)-1; j++)
            {

                if (fitnessArray[j + 1] < fitnessArray[j])
                {
                    itemp = chromosome[j];
                    chromosome[j] = chromosome[j + 1];
                    chromosome[j + 1] = itemp;
                    double dtemp = fitnessArray[j];
                    fitnessArray[j] = fitnessArray[j + 1];
                    fitnessArray[j + 1] = dtemp;
                }
            }
        }
    }

    int nchildren = (int)((1.0 - selection_rate) * population);
    if (!(nchildren % 2 == 0))
        nchildren++;
    int count_children = 0;
    Data parent0, parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    //#pragma omp parallel for private(t, parent0, parent1, count_children) num_threads(subCluster)
    for (t = 0; t < subCluster; t++)
    {

    for (; count_children < nchildren;)
    {
//#pragma omp critical
        {
        tournament(t, parent0, parent1);

        doubleCrossover(parent0, parent1, children[count_children], children[count_children + 1]);
        count_children += 2;
        }
    }
    }
    #pragma omp critical
    for (int i = 0; i < nchildren; i++)
    {
        chromosome[population - i - 1] = children[i];
    }

    for (int k = 0; k < subCluster; k++)
        newSum.at(k) = accumulate(&fitnessArray.at(this->subClusterStartPos(k)), &fitnessArray.at(this->subClusterEndPos(k)), 0);

    for (int k = 0; k < subCluster; k++)
    {
        newMO.at(k) = (double)newSum.at(k) / subCluster;
    }

    ++generation;
}

void pDoubleGenetic::init()
{
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
    population = double_chromosomes * subCluster;
    fitnessArray.resize(population);
    chromosome.resize(population);
    bestF2xInCluster.resize(subCluster);
    bestF2xInClusterOLD.resize(subCluster);

    for (int i = 0; i < population; i++)
    {

        chromosome[i] = myProblem->getRandomPoint();
        fitnessArray[i] = myProblem->funmin(chromosome[i]);
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
                // bestChromosomeInCluster.at(i) = chromosome[j];
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
    dmin = localSearch(bestx);
    after = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    auto ms = milliseconds.count();
    // std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;

    this->allmeans.clear();
  //  if (kmeans != 0)
  //      delete kmeans;
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

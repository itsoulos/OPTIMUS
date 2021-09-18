#include "de.h"

DE::DE(Problem *p)
    : Optimizer(p)
{
	printf("Now start DE \n");
    addParameter("population_count", "100", "Number of population");
    addParameter("max_generations", "30", "Maximum number of generations");
}
double DE::randMToN(double M, double N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
}
bool DE::terminated()
{
    int max_generations = params["max_generations"].toString().toInt();

    return generation >= max_generations;
}
void    DE::calcFitnessArray()
{  

}
void DE::selectAndCrossover()
{

    Data minCost = population[0];
    int bestMinIndex = 0;
    for (int i = 0; i < population_count; i++)
    {
        a = i;
        b = i;
        c = i;
	printf(" i %d\n",i);
        while (a == x || b == x || c == x || a == b || a == c || b == c)
        {
            a = rand() % population_count;
            b = rand() % population_count;
            c = rand() % population_count;
        }

        // Form intermediate solution z
        Data z(population_size);
        for (int j = 0; j < population_size; j++)
        {
            z[j] = population[a][j] + differentialWeight * (population[b][j] - population[c][j]);
        }

        // Chose random R        
        int R = rand() % population_count;

        // Chose random r for each dimension
        Data r(population_size);

        for (double &var : r)
        {
            var = randMToN(0,1);
        }

        Data newX(population_size);


        for (int j = 0; j < population_size; j++)
        {
            if (r[j] < crossoverProbability || j == R)
            {
                newX[j] = z[j];
            }
            else
            {
                newX[j] = population[i][j];
            }
        }
        for (int j = 0; j < population_size; j++)
        {
            if (newX[j]> lmargin[j] || newX[j] < rmargin[j])
            {

            }
        }
        double newMin = myProblem->funmin(newX);
        if (newMin < fitness_array[i])
        {
            population[i]=newX;
            fitness_array[i]=newMin;
        }
    }
}

void DE::step()
{
    calcFitnessArray();
    selectAndCrossover();
    ++generation;
}

void DE::init()
{
    generation = 0;
    crossoverProbability = 0.9;
    differentialWeight = 0.8;
    bestMinIndex=0;
    population_count = params["population_count"].toString().toInt();
	printf("START DE \n");
    population_size = myProblem->getDimension();

    population.resize(population_count);

    for (int i = 0; i < population_count; i++)
    {
        population[i].resize(population_size);
    }
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();

    for (int i = 0; i < population_count; i++)
    {
        population[i] = myProblem->getRandomPoint();
    }
    fitness_array.resize(population_count);
    for (int i = 0; i < population_count; i++)
    {
        fitness_array[i] = myProblem->funmin(population[i]);
        if (fitness_array[i] < bestMin)
        {
            bestMin = fitness_array[i];
            bestMinIndex = i;
        }
    }
}

void DE::done()
{

}
DE::~DE()
{
}
extern "C" DE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new DE(p);
}

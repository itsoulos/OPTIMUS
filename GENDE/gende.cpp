#include "gende.h"
#include "gende.h"

GENDE::GENDE(Problem *p)
    : Optimizer(p)
{
    prin = std::chrono::system_clock::now();
    addParameter("population_count", "100", "Number of population");
    addParameter("max_generations", "1000", "Maximum number of generations");
    addParameter("crossoverProbability", "0.9", "Crossover Probability");
    addParameter("differentialWeight", "0.8", "Differential Weight");
    addParameter("dimension", "10000", "Dimension");
}
double GENDE::randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}
bool GENDE::terminated()
{
    int max_generations = params["max_generations"].toString().toInt();
    double fmin = fabs(1.0 + fabs(bestMin));
    x1 = x1 + fmin;
    x2 = x2 + fmin * fmin;
    variance = (x2 / (generation + 1) - (x1 / (generation + 1)) * (x1 / (generation + 1)));
    variance = fabs(variance);

    if (bestMin < oldMin)
    {
        stopat = variance / 2.0;
        oldMin = bestMin;
    }
    printf("DE. Gen: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n", generation, bestMin, variance, stopat);
    if (stopat < 1e-8 && generation >= 10)
        return true;
    //return generation >= max_generations || (variance <= stopat && generation >= 20);
    return generation >= max_generations || (variance <= stopat);
}
void GENDE::selectAndCrossover()
{
    Data newX;
    double newMin;

    for (it = 0; it < population_count; it++)
    {
        int a = it;
        int b = it;
        int c = it;

        while (a == it || b == it || c == it || a == b || a == c || b == c)
        {
            a = rand() % population_count;
            b = rand() % population_count;
            c = rand() % population_count;
	    if(a<0) a=it;
	    if(b<0) b=it;
	    if(c<0) c=it;
        }

        Data z;
        z.resize(population_size);
//#pragma omp parallel for num_threads(threads)
        for (int j = 0; j < population_size; j++)
	{
            z[j] = population[a][j] + differentialWeight * (population[b][j] - population[c][j]);
	}
	//if(!myProblem->isPointIn(z)) z=population[a];
	    int R = rand() % population_size;

        Data r;
        r.resize(population_size);
	for(int i=0;i<population_size;i++) r[i]=drand48();

        newX.resize(population_size);
        bool flag = false;
//#pragma omp parallel for num_threads(threads)
        for (int j = 0; j < population_size; j++)
        {
            if (r[j] < crossoverProbability || j == R && (z[j]>=lmargin[j] && z[j]<=rmargin[j]))
            {

                newX[j] = z[j];
                flag = true;
            }
            else
                newX[j] = population[it][j];
        }
//#pragma omp parallel for num_threads(threads)
        for (int j = 0; j < population_size; j++)
        {
            if (newX[j] > rmargin[j])
                newX[j] = population[it][j];// rmargin[j];
            if (newX[j] < lmargin[j])
                newX[j] = population[it][j];//lmargin[j];
        }

        if (flag == true) {
            newMin = myProblem->funmin(newX);
		if(rand()%100==0)
	   Solv->Solve(newX,newMin);
        }
        else
	{
            newMin = fitness_array[it];
	}



        if (newMin < fitness_array[it])
        {
            population[it] = newX;
            fitness_array[it] = newMin;
	}
	if(newMin<bestMin)
	{

//	   Solv->Solve(newX,newMin);
//	   population[it]=newX;
    	bestMin = localSearch(newX);
	population[it]=newX;
            bestMin = newMin;
            bestMinIndex =it;
            bestPoint = population[it];
        }

        //bestMin = *min_element(fitness_array.begin(), fitness_array.end());
        //bestMinIndex = std::min_element(fitness_array.begin(), fitness_array.end()) - fitness_array.begin();
        //bestPoint = population[bestMinIndex];
    }
}

void GENDE::step()
{
    selectAndCrossover();
    ++generation;
}

void GENDE::init()
{
    generation = 1;
    oldMin = 1e+100;
    bestMin = 1e+100;;
    x1 = 0.0;
    x2 = 0.0;
    variance = 0.0;
    stopat = 0.0;
    bestMinIndex = 0;
    crossoverProbability = params["crossoverProbability"].toString().toDouble();
    differentialWeight = params["differentialWeight"].toString().toDouble();
    population_count = params["population_count"].toString().toInt();
    assert(population_count >= 4);
    dimension = params["dimension"].toString().toInt();
    population_size = myProblem->getDimension();
    assert(population_size > 1);
    if (population_size <= 0)
        population_size = dimension;
//  population_count = 20 * population_size;
    population.resize(population_count);
    for (int i = 0; i < population_count; i++)
        population[i].resize(population_size);
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    for (int i = 0; i < population_count; i++)
        population[i] = myProblem->getRandomPoint();
    fitness_array.resize(population_count);
    for (int i = 0; i < population_count; i++)
        fitness_array[i] = myProblem->funmin(population[i]);
    Solv = new Grs(myProblem);
    Solv->setGenomeCount(20);
    Solv->setGenomeLength(10 * population_size);
}
void GENDE::done()
{
    //bestMin =myProblem->funmin(bestPoint);
    bestMin = localSearch(bestPoint);
    meta = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(meta - prin);
    auto ms = milliseconds.count();

    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
}
GENDE::~GENDE()
{
    delete Solv;
}
extern "C" GENDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new GENDE(p);
}

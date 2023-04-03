#include "gende.h"
#include "gende.h"

int total_points = 0;
int total_failpoints = 0;

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
    bool ali = false;
    bool doubleboxflag = false;    
    bool charilogis = true;
    bool tsoulos =false; 

    int max_generations = params["max_generations"].toString().toInt();

    if (ali)
    {
        double dif = fabs(bestMin - bestMax);
        // printf("%4d] Generation  change: %10.6lf \n", generation, dd);
        if (dif < 1e-4)
        //    n++;
        //else
        //    n = 0;
        //if (n > 5)
            return true;

        return generation >= max_generations;
    }
    if (charilogis)
    {
        double dd = fabs(newSum - sum);
         printf("%4d] Generation  change: %10.6lf \n", generation, dd);
        sum = newSum;
        if (dd < 1e-8)
            n++;
        else
            n = 0;
        if (n > 20)
            return true;

        return generation >= max_generations;
    }
    if (doubleboxflag)
    {
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
        return generation >= max_generations || (variance <= stopat && generation >= 10);
    }
    if (tsoulos)
    {
        double dd = fabs(newSum - sum);
        printf("%4d] Generation  change: %10.6lf \n", generation, dd);
        if (dd < 1e-8)
            n++;
        else
            n = 0;
        // if (n>5)
        //     return true;
        // return generation >= max_generations;
        double fmin = newSum; // fabs(1.0 + fabs(bestMin));
        x1 = x1 + fmin;
        x2 = x2 + fmin * fmin;
        variance = (x2 / (generation + 1) - (x1 / (generation + 1)) * (x1 / (generation + 1)));
        variance = fabs(variance);

        if (newSum < sum) // bestMin < oldMin)
        {
            stopat = variance / 2.0;
            oldMin = bestMin;
        }
        sum = newSum;
        printf("DE. Gen: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n", generation, bestMin, variance, stopat);
        if (stopat < 1e-8 && generation >= 10)
            return true;
        return generation >= max_generations || (variance <= stopat && generation >= 10);
    }
}
void GENDE::selectAndCrossover()
{
    Data newX;
    double newMin;

    bool grsflag = false;
    bool bfgsflag = false;

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
            if (a < 0)
                a = it;
            if (b < 0)
                b = it;
            if (c < 0)
                c = it;
        }

        Data z;
        z.resize(population_size);
        //#pragma omp parallel for num_threads(threads)

        double wmin = 0.4;
        double wmax = 0.9;
        int max_generations = params["max_generations"].toString().toInt();
        double inertia = wmax - generation * 1.0 / max_generations * (wmax - wmin);

        //////////////////////tsoulos/////////////////////////
	bool tsoulos_flag=true;
	bool ali_flag=false;

	if(tsoulos_flag)
	{
       double alfa = -0.5 + 2.0 * myProblem->randomDouble();
       differentialWeight = alfa;
	}
	else
	if(ali_flag)
    	{	

        double f;
            if (bestMax/bestMin <1)
                f = 1.0 - (bestMax/bestMin);
            else
                f = 1.0 - (bestMin/bestMax);
        differentialWeight = f;
	}



        for (int j = 0; j < population_size; j++)
        {
            z[j] = population[a][j] + differentialWeight * (population[b][j] - population[c][j]);
        }
        // if(!myProblem->isPointIn(z)) z=population[a];
        int R = rand() % population_size;

        Data r;
        r.resize(population_size);
        for (int i = 0; i < population_size; i++)
            r[i] = drand48();

        newX.resize(population_size);
        bool flag = false;
        //#pragma omp parallel for num_threads(threads)
        for (int j = 0; j < population_size; j++)
        {
            if (r[j] < crossoverProbability || j == R && (z[j] >= lmargin[j] && z[j] <= rmargin[j]))
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
            total_points++;
            if (newX[j] > rmargin[j] || newX[j] < lmargin[j])
            {
                total_failpoints++;
                newX[j] = population[it][j];
            }
        }

        if (flag == true)
        {
            newMin = myProblem->funmin(newX);
            if (grsflag)
            {
            }
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
        if (newMin < bestMin)
        {

            bool grsflag = false;
            if (grsflag)
            {
            }
            if (bfgsflag)
            {
                bestMin = localSearch(newX);
            }
            population[it] = newX;
            bestMin = newMin;
            bestMinIndex = it;
            bestPoint = population[it];
            bestMax = *max_element(fitness_array.begin(), fitness_array.end());
        }

        // bestMin = *min_element(fitness_array.begin(), fitness_array.end());
        // bestMinIndex = std::min_element(fitness_array.begin(), fitness_array.end()) - fitness_array.begin();
        // bestPoint = population[bestMinIndex];
        newSum = accumulate(fitness_array.begin(), fitness_array.end(), 0);
        newSum = newSum / population_count;
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
    bestMin = 1e+100;
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
    population_count = 10 * population_size;
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
    sum = accumulate(fitness_array.begin(), fitness_array.end(), 0);
    if (sum == 0)
    {
        // assert(sum > 0);
    }
    sum = sum / population_count;
    //printf("\n   0] Generation  start : %10.6lf \n", sum);
    n = 0;
}
void GENDE::done()
{
    // bestMin =myProblem->funmin(bestPoint);
    bestMin = localSearch(bestPoint);
    meta = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(meta - prin);
    auto ms = milliseconds.count();

    std::cout << "Douration: " << (double)ms / 1000.0 << " sec" << std::endl;
    printf("FAILURE: %.2lf%%\n", total_failpoints * 100.0 / total_points);
}
GENDE::~GENDE()
{
}
extern "C" GENDE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new GENDE(p);
}

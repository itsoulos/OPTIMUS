#include "nelderMead.h"
nelderMead::nelderMead(Problem *p,Data &x,double y)

{
    myProblem = p;
    bestPoint.resize(x.size());
    bestPoint = x;
    ybestPoint=y;
    population_count = 200;
    alpha=1.0;
    gamma=2.0;
    ro = 0.5;
    sigma = 0.5;
    max_generations=100;

}
bool nelderMead::terminated()
{

    double dd = fabs(newSum - sum);
     printf("%4d] Generation  change: %10.6lf ybest:%10.6lf \n", generation, dd,ybestPoint);
    if (dd < 1e-8)
        n++;
    else
        n = 0;
    if (n > 15)
        return true;
    sum = newSum;

    return generation >= max_generations;
}

void nelderMead::order()
{
    sort(population.begin(), population.end());//[](auto a,auto b){return a.first < b.first;});
}
void nelderMead::center()
{

    for (int i = 0; i < population_size; i++)
    {
        centerPoint[i] = 0.0;
    }
    for (int i = 0; i < population_count-1; i++)
    {
        for (int j = 0; j < population_size; j++)
        {
            centerPoint[j] += population[i].second[j];
        }
    }
    for (int i = 0; i < population_size; i++)
    {
        centerPoint[i] /= population_count-1;
        if (centerPoint[i] < lmargin[i])
            centerPoint[i] = lmargin[i];
        if (centerPoint[i] > rmargin[i])
            centerPoint[i] = rmargin[i];
    }
}
void nelderMead::reflection()
{
    for (int j = 0; j < population_size; j++)
    {
        reflectedPoint[j] = centerPoint[j] + alpha * (centerPoint[j] - population[population_count - 1].second[j]);
    }
    yreflectedPoint = myProblem->funmin(reflectedPoint);
}
void nelderMead::expansion()
{
    for (int j = 0; j < population_size; j++)
    {
        expandedPoint[j] = centerPoint[j] + gamma * (reflectedPoint[j] - centerPoint[j]);
    }
    yexpandedPoint = myProblem->funmin(expandedPoint);
}
void nelderMead::contraction()
{
    for (int j = 0; j < population_size; j++)
    {
        contractedPoint[j] = centerPoint[j] + ro * (reflectedPoint[j] - centerPoint[j]);
    }
    ycontractedPoint = myProblem->funmin(contractedPoint);
}
void nelderMead::contractionB()
{
    for (int j = 0; j < population_size; j++)
    {
        contractedPoint[j] = centerPoint[j] + ro * (population[population_count - 1].second[j] - centerPoint[j]);
    }
    ycontractedPoint = myProblem->funmin(contractedPoint);
}
void nelderMead::shrink()
{
    for (int i = 1; i < population_count; i++)
    {
        for (int j = 0; j < population_size; j++)
        {
            population[i].second[j] = population[0].second[j] + sigma * (population[i].second[j] - population[0].second[j]);
            population[i].first = myProblem->funmin(population[i].second);
        }
    }
}
void nelderMead::step()
{
start:
    order();
  center();
    ybestPoint = population.begin()->first;
    worst = (population.end())->first;
    secondWorst = population[population_count - 2].first;
    reflection();
    if (yreflectedPoint < secondWorst && yreflectedPoint > ybestPoint)
    {
        population[population_count - 1].second = reflectedPoint;
        population[population_count - 1].first = yreflectedPoint;
        goto start;
    }
    else if (yreflectedPoint < ybestPoint)
    {
        expansion();
        if (yexpandedPoint < yreflectedPoint)
        {
            population[population_count - 1].second = expandedPoint;
            population[population_count - 1].first = yexpandedPoint;
            goto start;
        }
        else
        {
            population[population_count - 1].second = reflectedPoint;
            population[population_count - 1].first = yreflectedPoint;
            goto start;
        }
        goto start;
    }
    else
    {
        if (yreflectedPoint < worst)
        {
            contraction();
            if (ycontractedPoint < yreflectedPoint)
            {
                population[population_count - 1].second = contractedPoint;
                population[population_count - 1].first = ycontractedPoint;
                goto start;
            }
            else
                shrink();
        }
        else
        {
            if (ycontractedPoint < worst)
            {
                contractionB();
                population[population_count - 1].second = contractedPoint;
                population[population_count - 1].first = ycontractedPoint;
                goto start;
            }
            else
                shrink();
        }
    }
    newSum = accumulate(&population.begin()->first, &population.end()->first, 0);
    newSum = newSum / population_count;
    ++generation;
}

void nelderMead::init()
{
    generation = 0;
    assert(population_count >= 4);
    population_size = myProblem->getDimension();
    population.resize(population_count);
    for (int i = 0; i < population_count; i++)
    {
        population[i].second.resize(population_size);
    }
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    centerPoint.resize(population_size);
    reflectedPoint.resize(population_size);
    expandedPoint.resize(population_size);
    contractedPoint.resize(population_size);
    for (int i = 0; i < population_count; i++)
    {
        if(i==0)
        {
            population[i].second = bestPoint;
            population[i].first = ybestPoint;
        }
        else
        {
            population[i].second = myProblem->getRandomPoint();
            population[i].first = myProblem->funmin(population[i].second);
        }
    }
    sum = accumulate(&population.begin()->first, &population.end()->first, 0);
    // if (sum == 0)
    //     assert(sum > 0);
    sum = sum / population_count;
    n = 0;
}

Data nelderMead::getBestX() const
{
    return bestPoint;
}

double nelderMead::getBestY() const
{
    return ybestPoint;
}

void nelderMead::done()
{
    ybestPoint = myProblem->funmin(population[0].second);
}
nelderMead::~nelderMead()
{
}

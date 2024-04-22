#include "PM.h"

PM::PM(Problem *p) : Optimizer(p)
{
    addParameter("optimization", "1", "Optimization Method [1=>DE, 2=>PSO, 3=>MULT, 4=>DEPSO, 5=>DEMULT, 6=>PSOMULT, 7=>DEPSOMULT]");
    addParameter("population", "100", "Number of population");
    addParameter("maxiterations", "200", "Maximum iterations");
    addParameter("stoppingrule", "doublebox", "Termination criterion (mean,best,doublebox,ali)");
    addParameter("similarityMaxCount", "15", "Maximum allowed itearations for Similarity Stopping rule");
    addParameter("differentialWeight", "0.8", "differentialWeight");
    addParameter("crossoverProbability", "0.9", "crossoverProbability");
    addParameter("c1", "0.5", "Pso c1 parameter");
    addParameter("c2", "0.5", "Pso c2 parameter");
    addParameter("localRate", "0.01", "Local search rate");
}

bool PM::terminated()
{

    if (stoppingrule == "mean" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double difference1 = fabs(newSumDE - sumDE);            
            newSumDE = sumDE;
            if (difference1 < 1e-6)
                similarityCurrentCountMean_DE++;
            else
                similarityCurrentCountMean_DE = 0;

            if (similarityCurrentCountMean_DE > similarityMaxCount)
                return true;
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double difference2 = fabs(newSumPSO - sumPSO);
            newSumPSO=sumPSO;
            if (difference2 < 1e-6)
                similarityCurrentCountMean_PSO++;
            else
                similarityCurrentCountMean_PSO = 0;
            if (similarityCurrentCountMean_PSO > similarityMaxCount)
                return true;
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double difference3 = fabs(newSumMULT- sumMULT);
            sumMULT = newSumMULT;
            if (difference3 < 1e-6)
                similarityCurrentCountMean_MULT++;
            else
                similarityCurrentCountMean_MULT = 0;
            if (similarityCurrentCountMean_MULT > similarityMaxCount)
                return true;
        }
    }
    if (stoppingrule == "best" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double difference4 = fabs(besty_DE_tmp - besty_DE);        
            besty_DE_tmp = besty_DE;
            if (difference4 < 1e-6)
                similarityCurrentCountBest_DE++;
            else
                similarityCurrentCountBest_DE = 0;
            if (similarityCurrentCountBest_DE > similarityMaxCount)
                return true;
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double difference5 = fabs(besty_PSO_tmp - besty_PSO);
            besty_PSO_tmp = besty_PSO;
            if (difference5 < 1e-6)
                similarityCurrentCountBest_PSO++;
            else
                similarityCurrentCountBest_PSO = 0;
            if (similarityCurrentCountBest_PSO > similarityMaxCount)
                return true;
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double difference6 = fabs(besty_MULT_tmp - besty_MULT);
            besty_MULT_tmp = besty_MULT;
            if (difference6 < 1e-6)
                similarityCurrentCountBest_MULT++;
            else
                similarityCurrentCountBest_MULT = 0;
            if (similarityCurrentCountBest_MULT > similarityMaxCount)
                return true;
        }
    }
    if (stoppingrule == "doublebox" || stoppingrule == "all")
    {
        if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
        {
            double fmin = fabs(1.0 + fabs(besty_DE));
            if (iter <= 1)
            {
                x1_DE = 0.0;
                x2_DE = 0.0;
            }
            x1_DE += fmin;
            x2_DE += fmin * fmin;
            variance_DE = x2_DE / (iter + 1) - (x1_DE / (iter + 1)) * (x1_DE / (iter + 1));
            variance_DE = fabs(variance_DE);
            if (besty_DE < besty_DE_old)
            {
                besty_DE_old = besty_DE;
                stopat_DE = variance_DE / 2.0;
            }
            if (stopat_DE < 1e-8 && iter > similarityMaxCount)
                return true;
            return iter >= maxiterations || (variance_DE <= stopat_DE && iter > similarityMaxCount);
        }
        if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
        {
            double fmin = fabs(1.0 + fabs(besty_PSO));
            if (iter <= 1)
            {
                x1_PSO = 0.0;
                x2_PSO = 0.0;
            }
            x1_PSO += fmin;
            x2_PSO += fmin * fmin;
            variance_PSO = x2_PSO / (iter + 1) - (x1_PSO / (iter + 1)) * (x1_PSO / (iter + 1));
            variance_PSO = fabs(variance_PSO);
            if (fmin < besty_PSO_old)
            {
                besty_PSO_old = fmin;
                stopat_PSO = variance_PSO / 2.0;
            }
            if (stopat_PSO < 1e-8 && iter > similarityMaxCount)
                return true;
            return iter >= maxiterations || (variance_PSO <= stopat_PSO && iter > similarityMaxCount);
        }
        if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
        {
            double fmin = fabs(1.0 + fabs(besty_MULT));
            if (iter <= 1)
            {
                x1_MULT = 0.0;
                x2_MULT = 0.0;
            }
            x1_MULT += fmin;
            x2_MULT += fmin * fmin;
            variance_MULT = x2_MULT / (iter + 1) - (x1_MULT / (iter + 1)) * (x1_MULT / (iter + 1));
            variance_MULT = fabs(variance_MULT);
            if (besty_MULT< besty_MULT_old)
            {
                besty_MULT_old = besty_MULT;
                stopat_MULT = variance_MULT / 2.0;
            }
            if (stopat_MULT < 1e-8 && iter > similarityMaxCount)
                return true;
            return iter >= maxiterations || (variance_MULT <= stopat_MULT && iter > similarityMaxCount);
        }
    }

    return iter >= maxiterations;
}

double PM::randMToN(double M, double N)
{
    return M + (rand() / (RAND_MAX / (N - M)));
}

void PM::updateWithDE()
{
    Data newX;
    double newMin;
    int it;
    for (it = 0; it < population; it++)
    {
        int a = it;
        int b = it;
        int c = it;

        while (a == it || b == it || c == it || a == b || a == c || b == c)
        {
            a = rand() % population;
            b = rand() % population;
            c = rand() % population;
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
            z[j] = elements[a][j] + differentialWeight * (elements[b][j] - elements[c][j]);
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
                newX[j] = elements[it][j];
        }

        if (flag)
        {
            newMin = myProblem->funmin(newX);
            if (localRate > 0 && myProblem->randomDouble() <= localRate)
            {
                fitness_arrayDE[it] = localSearch(elements.at(it));
            }
        }
#pragma omp critical
{
        if (newMin < fitness_arrayDE[it])
        {
            elements[it] = newX;
            fitness_arrayDE[it] = newMin;
        }
        if (newMin < besty_DE)
        {
            elements[it] = newX;
            besty_DE = newMin;
            bestx_DE = elements[it];
            best(it);
        }

}
    }

}
void PM::updateWithPso()
{

    double inertia, R;
    // besty_PSO_tmp = besty_PSO;
    for (int i = 0; i < population; i++)
    {

        bool flag = false;
        Data oldg = particles[i];
        for (int j = 0; j < dimension; j++)
        {
            R = randMToN(0.0, 1.0);
            inertia = 0.5 + (R / 2.0);
            double r1 = randMToN(0.0, 1.0);
            double r2 = randMToN(0.0, 1.0);
            double tj = velocity[i][j];
            double part1 = inertia * velocity[i][j];
            double part2 = c1 * r1 * (bestPoints_PSO[i][j] - particles[i][j]);
            double part3 = c2 * r2 * (bestx_PSO[j] - particles[i][j]);
            velocity[i][j] = part1 + part2 + part3;
            double trialf = particles[i][j] + velocity[i][j];

            if (trialf < lmargin[j] || trialf > rmargin[j])
            {
                velocity[i][j] = tj;
                continue;
            }
            else
            {
                particles[i][j] = trialf;
                flag = true;
            }
        }

        if (flag){
            fitness_arrayPSO[i] = myProblem->funmin(particles.at(i));
            if (localRate > 0 && myProblem->randomDouble() <= localRate)
            {
                fitness_arrayPSO[i] = localSearch(particles.at(i));
            }
        }
    }

    for (int i = 0; i < population; i++)
    {
#pragma omp critical
{
        if (fitness_arrayPSO[i] < bestFitness_arrayPSO[i])
        {
            bestFitness_arrayPSO[i] = fitness_arrayPSO[i];
            bestPoints_PSO[i] = particles[i];
        }

        if (besty_PSO > bestFitness_arrayPSO[i])
        {
            bestx_PSO = bestPoints_PSO[i];
            besty_PSO = bestFitness_arrayPSO[i];
            best(i);
        }
}
    }
}

void PM::updateWithMultistart()
{
    // besty_MULT_tmp = besty_MULT;
    for (int i = 0; i < points.size(); i++)
    {
        fitness_arrayMULT.at(i) = localSearch(points.at(i));

        for (int i = 0; i < population; i++)
        {
#pragma omp critical
            if (besty_MULT > fitness_arrayMULT[i])
            {
                bestx_MULT = points[i];
                besty_MULT = fitness_arrayMULT[i];
                best(i);
            }
        }
    }
}

void PM::step()
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
#pragma omp parallel sections
    {
#pragma omp section
        {
            if (optimization == 1 || optimization == 4 || optimization == 5 || optimization == 7)
                updateWithDE();
        }
#pragma omp section
        {
            if (optimization == 2 || optimization == 4 || optimization == 6 || optimization == 7)
                updateWithPso();
        }
#pragma omp section
        {
            if (optimization == 3 || optimization == 5 || optimization == 6 || optimization == 7)
                updateWithMultistart();
        }
    }

    sumDE = accumulate(fitness_arrayDE.begin(), fitness_arrayDE.end(), 0);
    sumPSO = accumulate(bestFitness_arrayPSO.begin(), bestFitness_arrayPSO.end(), 0);
    sumMULT = accumulate(fitness_arrayMULT.begin(), fitness_arrayMULT.end(), 0);
    sumDE = sumDE / population;
    sumPSO = sumDE / population;
    sumMULT = sumMULT / population;

}
void PM::best(int pos)
{
    if (besty_DE<besty_PSO && besty_DE<besty_MULT)
    {
        besty_PSO=besty_DE;
        bestx_PSO=bestx_DE;
        fitness_arrayPSO[pos]=besty_DE;
        bestFitness_arrayPSO[pos]=besty_DE;
        besty_MULT=besty_DE;
        bestx_MULT=bestx_DE;
        fitness_arrayMULT[pos]=besty_DE;

    }
    if (besty_PSO<besty_DE && besty_PSO<besty_MULT)
    {
        besty_DE=besty_PSO;
        bestx_DE=bestx_PSO;
        fitness_arrayDE[pos]=besty_PSO;
        besty_MULT=besty_PSO;
        bestx_MULT=bestx_PSO;
        fitness_arrayMULT[pos]=besty_PSO;
    }
    if (besty_MULT<besty_DE && besty_MULT<besty_PSO)
    {
        besty_DE=besty_MULT;
        bestx_DE=bestx_MULT;
        fitness_arrayDE[pos]=besty_MULT;        
        besty_PSO=besty_MULT;
        bestx_PSO=bestx_MULT;
        fitness_arrayPSO[pos]=besty_MULT;
        bestFitness_arrayPSO[pos]=besty_MULT;
    }
}
void PM::bestAll(int pos)
{
    if (besty_DE<besty_PSO && besty_DE<besty_MULT)
    {
        besty_PSO=besty_DE;
        bestx_PSO=bestx_DE;
        fitness_arrayPSO[pos]=besty_DE;
        bestFitness_arrayPSO[pos]=besty_DE;
        besty_MULT=besty_DE;
        bestx_MULT=bestx_DE;
        fitness_arrayMULT[pos]=besty_DE;

    }
    if (besty_PSO<besty_DE && besty_PSO<besty_MULT)
    {
        besty_DE=besty_PSO;
        bestx_DE=bestx_PSO;
        fitness_arrayDE[pos]=besty_PSO;
        besty_MULT=besty_PSO;
        bestx_MULT=bestx_PSO;
        fitness_arrayMULT[pos]=besty_PSO;
    }
    if (besty_MULT<besty_DE && besty_MULT<besty_PSO)
    {
        besty_DE=besty_MULT;
        bestx_DE=bestx_MULT;
        fitness_arrayDE[pos]=besty_MULT;
        fitness_arrayDE[pos]=besty_MULT;
        besty_PSO=besty_MULT;
        bestx_PSO=bestx_MULT;
        fitness_arrayPSO[pos]=besty_MULT;
    }

}
void PM::init()
{
    localRate = params["localRate"].toString().toDouble();
    optimization = params["optimization"].toString().toInt();
    similarityMaxCount = params["similarityMaxCount"].toString().toInt();
    stoppingrule = params["stoppingrule"].toString();
    maxiterations = params["maxiterations"].toString().toInt();
    population = params["population"].toString().toInt();

        switch (optimization)
        {
            case 1:
            case 2:
            case 3:population /= 1;break;
            case 4:
            case 5:
            case 6:population /= 2;break;
            case 7:population /= 3;break;
            default:{}
        }

    c1 = params["c1"].toString().toDouble();
    c2 = params["c2"].toString().toDouble();
    crossoverProbability = params["crossoverProbability"].toString().toInt();
    differentialWeight = params["differentialWeight"].toString().toInt();
    stoppingrule = params["stoppingrule"].toString();
    dimension = myProblem->getDimension();
    elements.resize(population);
    particles.resize(population);
    points.resize(population);
    bestPoints_PSO.resize(population);
    velocity.resize(population);
    fitness_arrayDE.resize(population);
    fitness_arrayPSO.resize(population);
    bestFitness_arrayPSO.resize(population);
    fitness_arrayMULT.resize(population);
    bestx_DE.resize(myProblem->getDimension());
    bestx_PSO.resize(myProblem->getDimension());
    bestx_MULT.resize(myProblem->getDimension());
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    iter = 0;
    besty_DE = besty_PSO = besty_MULT = besty_DE_tmp = besty_PSO_tmp = besty_PSO_tmp = besty_DE_old = besty_PSO_old = besty_MULT_old =1e+100;
    similarityCurrentCountMean_DE = 0;
    similarityCurrentCountBest_DE = 0;
    similarityCurrentCountMean_PSO = 0;
    similarityCurrentCountBest_PSO = 0;
    similarityCurrentCountMean_MULT = 0;
    similarityCurrentCountBest_MULT = 0;
    x1_DE = 0.0;
    x2_DE = 0.0;
    stopat_DE = 0.0;
    variance_DE = 0.0;
    x1_PSO = 0.0;
    x2_PSO = 0.0;
    stopat_PSO = 0.0;
    variance_PSO = 0.0;
    x1_MULT = 0.0;
    x2_MULT = 0.0;
    stopat_MULT = 0.0;
    variance_MULT = 0.0;
    bestPoints_PSO.resize(myProblem->getDimension());

    for (int i = 0; i < population; i++)
    {
        points[i].resize(myProblem->getDimension());
        particles[i].resize(myProblem->getDimension());
        elements[i].resize(myProblem->getDimension());
        velocity[i].resize(myProblem->getDimension());
        points[i] = myProblem->getRandomPoint();
        particles[i] = points[i];
        elements[i] = points[i];
        bestPoints_PSO[i] = points[i];
        fitness_arrayDE[i] = myProblem->funmin(points[i]);
        fitness_arrayPSO[i] = fitness_arrayDE[i];
        bestFitness_arrayPSO[i] = fitness_arrayDE[i];
        fitness_arrayMULT[i] = fitness_arrayDE[i];

        for (int j = 0; j < dimension; j++)
        {
            double left = -(rmargin[j] - lmargin[j]) / 20.0;
            double right = (rmargin[j] - lmargin[j]) / 20.0;
            velocity[i][j] = left + myProblem->randomDouble() * (right - left);
            velocity[i][j] = 0;
        }
    }

    sumDE = accumulate(fitness_arrayDE.begin(), fitness_arrayDE.end(), 0.0);
    sumPSO = accumulate(fitness_arrayPSO.begin(), fitness_arrayPSO.end(), 0.0);
    sumMULT = accumulate(fitness_arrayMULT.begin(), fitness_arrayMULT.end(), 0.0);
    sumDE = sumDE / population;
    sumPSO = sumPSO / population;
    sumMULT = sumMULT / population;



}

void PM::done()
{
    if (optimization == 1)
        besty_DE = localSearch(bestx_DE);
    else if(optimization == 2)
        besty_PSO = localSearch(bestx_PSO);
    else if(optimization == 3)
        besty_MULT = localSearch(bestx_MULT);
    else if(optimization == 4)
    {
        if (besty_DE<besty_PSO)
            besty_DE = localSearch(bestx_DE);
        else
            besty_PSO = localSearch(bestx_PSO);
    }
    else if(optimization == 5)
    {
        if (besty_DE<besty_MULT)
            besty_DE = localSearch(bestx_DE);
        else
            besty_MULT = localSearch(bestx_MULT);
    }
    else if(optimization == 6)
    {
        if (besty_PSO<besty_MULT)
            besty_PSO = localSearch(bestx_PSO);
        else
            besty_MULT = localSearch(bestx_MULT);
    }
    else if(optimization == 7)
    {
        if (besty_DE<besty_PSO && besty_DE<besty_MULT)
            besty_DE = localSearch(bestx_DE);
        else if (besty_PSO<besty_DE && besty_PSO<besty_MULT)
            besty_PSO = localSearch(bestx_PSO);
        else
            besty_MULT = localSearch(bestx_MULT);
    }




}

double PM::fitness(Data &x)
{
    if (!myProblem->isPointIn(x))
        return 1e+100;
    return myProblem->funmin(x);
}

void PM::Solve()
{
    Optimizer::Solve();
}

PM::~PM()
{
}

extern "C" PM_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new PM(p);
}

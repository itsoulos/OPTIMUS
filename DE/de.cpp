#include "de.h"

DE::DE(Problem *p)
    : Optimizer(p)
{
    addParameter("population_count", "100", "Number of population");
    addParameter("max_generations", "2000", "Maximum number of generations");
    addParameter("de_method","delb","Available values: de,derl,delb");
}

int  DE::tournament(int tsize)
{
    int index =-1;
    double bestValue = 1e+100;
    for(int i=0;i<tsize;i++)
    {
        int pos = rand() % population_count;
        if(i==0 || fitness_array[pos]<bestValue)
        {
            index = pos;
            bestValue = fitness_array[pos];
        }
    }
    return index;
}

double DE::randMToN(double M, double N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
}
bool DE::terminated()
{
    int max_generations = params["max_generations"].toString().toInt();
    double fmin,fmax;
    getBestWorst(fmin,fmax);
    return (fmax-fmin)<1e-4 || generation >= max_generations;
}

int DE::getBestIndex()
{
    int index=0;
    for(int i=0;i<population_count;i++)
    {
        if(fitness_array[i]<fitness_array[index])
            index = i;
    }
    return index;
}
void DE::selectAndCrossover()
{
    QString method = params["de_method"].toString();
    Data newX(population_size);

    for (int i = 0; i < population_count; i++)
    {
        a = i;
        b = i;
        c = i;
        while (a == x || b == x || c == x || a == b || a == c || b == c)
        {
            if(method == "derl")
                a=tournament();
            else
                a = rand() % population_count;
            b = rand() % population_count;
            c = rand() % population_count;
        }

        // Form intermediate solution z
        Data z(population_size);
        for (int j = 0; j < population_size; j++)
        {
            if(method=="derl"  || method == "delb")
              {
                double fi=0.0;
                int r = rand() %2;
                if(r==0) fi = -1.0+(-0.4+1.0)*drand48();
                else  fi = 0.4 + (1.0-0.4)*drand48();
                z[j] = population[a][j] + fi * (population[b][j] - population[c][j]);
            }
            else
            z[j] = population[a][j] + differentialWeight * (population[b][j] - population[c][j]);
        }

        // Chose random R        
        int R = rand() % population_size;

        // Chose random r for each dimension
        Data r(population_size);

        for (double &var : r)
        {
            var = randMToN(0,1);
        }



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
        bool insideflag = true;
        for (int j = 0; j < population_size; j++)
        {
            if (newX[j]< lmargin[j] || newX[j] > rmargin[j])
            {
                insideflag = false;
                break;
            }
        }
        if(method == "delb")
        {
            if(!insideflag)
            {
                ypopulation[i]=population[i];
                yfitness_array[i]=fitness_array[i];
            }
            else
            {
                ypopulation[i]=newX;
                yfitness_array[i]=myProblem->funmin(newX);
            }
        }
        else
        {
            if(!insideflag) continue;
            double newMin = myProblem->funmin(newX);
            if (newMin < fitness_array[i])
            {
                population[i]=newX;
                fitness_array[i]=newMin;
            }
        }
    }
    //step 4
    if(method=="delb")
    {

       for(int i=0;i<population_count;i++)
       {
           double fmin;
           double fri,fci;
           bool stopflag = false;
           int bestIndex= getBestIndex();
           fmin = fitness_array[bestIndex];
           step4a:
           if(yfitness_array[i]>fitness_array[i]) continue;
           else
           {
                double ri=drand48();
                if(ri<parameterW && yfitness_array[i]>fmin)
                goto step4b; else goto step4d;
           }
           step4b:
           stopflag=false;
           for(int j=0;j<population_size;j++)
           {
               newX[j]=population[bestIndex][j]-(ypopulation[i][j]-population[bestIndex][j]);
               if(newX[j]<lmargin[j] || newX[j]>rmargin[j])
               {
                   stopflag=true;
                   break;
               }
           }
           if(stopflag) continue;

           fri=myProblem->funmin(newX);
           if(fri<yfitness_array[i])
           {
               population[i]=newX;
               fitness_array[i]=fri;
               continue;
           }
           step4c:
           stopflag = false;
           for(int j=0;j<population_size;j++)
           {
               newX[j]=population[bestIndex][j]+0.5*(ypopulation[i][j]-population[bestIndex][j]);

               if(newX[j]<lmargin[j] || newX[j]>rmargin[j])
               {
                   stopflag=true;
                   break;
               }
           }
           if(stopflag) continue;

           fci = myProblem->funmin(newX);
           if(fci<yfitness_array[i])
           {
               population[i]=newX;
               fitness_array[i]=fci;
               continue;
           }
           step4d:
           population[i]=ypopulation[i];
           fitness_array[i]=yfitness_array[i];
       }
    }
}

void    DE::getBestWorst(double &fmin,double &fmax)
{
    fmin  = fitness_array[0];
    fmax  = fitness_array[0];
    for(int i=0;i<population_count;i++)
    {
        if(fitness_array[i]<fmin) fmin=fitness_array[i];
        if(fitness_array[i]>fmax) fmax=fitness_array[i];
    }
}

void DE::step()
{
    selectAndCrossover();
    ++generation;
}

void DE::init()
{
    generation = 0;
    crossoverProbability = 0.9;
    differentialWeight = 0.8;
    parameterW  = 0.25;
    bestMinIndex=0;
    population_count = params["population_count"].toString().toInt();
    population_size = myProblem->getDimension();
    population.resize(population_count);
    ypopulation.resize(population_count);
    for (int i = 0; i < population_count; i++)
    {
        population[i].resize(population_size);
        ypopulation[i].resize(population_size);
    }
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();

    for (int i = 0; i < population_count; i++)
    {
        population[i] = myProblem->getRandomPoint();
    }
    fitness_array.resize(population_count);
    yfitness_array.resize(population_count);
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
    int bestIndex = -1;
    double bestValue=1e+100;
    for(int i=0;i<population_count;i++)
    {
        if(i==0 || fitness_array[i]<bestValue)
        {
            bestIndex = i;
            bestValue = fitness_array[i];
        }
    }
    fitness_array[bestIndex]=localSearch(population[bestIndex]);
}

DE::~DE()
{
}
extern "C" DE_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new DE(p);
}

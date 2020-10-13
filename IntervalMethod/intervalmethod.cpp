#include "intervalmethod.h"
IntervalMethod::IntervalMethod(Problem *p)
    :Optimizer(p)
{
    addParameter("interval_chromosomes","200","Number of chromosomes");
    addParameter("interval_generations","200","Maximum number of generations");
    addParameter("interval_selection_rate","0.1","Selection rate");
    addParameter("interval_mutation_rate","0.05","Mutation rate");
    addParameter("interval_localsearch_rate","0.0","Local search rate");
    addParameter("interval_samples","25","Number of interval samples");
}

void IntervalMethod::Solve()
{
  Optimizer::Solve();
}

void    IntervalMethod::printMargin(Data &x)
{
    printf("Margin=[");
    for(int j=0;j<myProblem->getDimension();j++)
    {
        double delta=rmargin[j]-lmargin[j];
        double left=lmargin[j]+x[j*2+0]*delta;
        double right=rmargin[j]-x[j*2+1]*delta;

        if(left>right)
        {
            double t=left;
            left=right;
            right=left;
        }
        printf("(%lf,%lf)",left,right);
        if(j!=myProblem->getDimension()-1)
            printf(",");
    }
    printf("]\n");
}

bool IntervalMethod::terminated()
{
    int max_generations=params["interval_generations"].toString().toInt();
    double fmin=fabs(1.0+fabs(fitness_array[0]));

    x1=x1+fmin;
    x2=x2+fmin * fmin;


    variance = x2/(generation+1) -(x1/(generation+1))*(x1/(generation+1));
    variance=fabs(variance);

    if(fitness_array[0]<oldBesty)
    {
        oldBesty=fitness_array[0];

        stopat=variance/2.0;
    }


    if(stopat<1e-8 && generation>=10) return true;
    printf("Generation fit: %d value: %lf variance: %lf stopat: %lf\n",
           generation,
            fitness_array[0],variance,stopat);
    printMargin(chromosome[0]);


    return generation>=max_generations || (variance<=stopat && generation>=10);
}

void    IntervalMethod::step()
{
    if(generation) mutate();
    calcFitnessArray();
    select();
    crossover();
    ++generation;
}

void    IntervalMethod::init()
{
    generation=0;
    x1=0.0;
    x2=0.0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;

    genome_count=params["interval_chromosomes"].toString().toInt();
    genome_size=2 *  myProblem->getDimension();
    chromosome.resize(genome_count);
    int nsamples=params["interval_samples"].toString().toInt();
    randNumber.resize(nsamples);
    for(int i=0;i<nsamples;i++)
    {
        randNumber[i].resize(myProblem->getDimension());
        for(int j=0;j<myProblem->getDimension();j++)
            randNumber[i][j]=myProblem->randomDouble();
    }
    fitness_array.resize(genome_count);
    children.resize(genome_count);

    parent0.resize(genome_size);
    parent1.resize(genome_size);

    lmargin=myProblem->getLeftMargin();
    rmargin=myProblem->getRightMargin();

    for(int i=0;i<genome_count;i++)
     {
            chromosome[i].resize(genome_size);
            children[i].resize(genome_size);
            for(int j=0;j<genome_size;j++)
                chromosome[i][j]=myProblem->randomDouble();
            fitness_array[i]=fitness(chromosome[i]);
     }

}

void   IntervalMethod::getInterval(Data &x,Data &leftArray,Data &rightArray)
{
    leftArray.resize(myProblem->getDimension());
    rightArray.resize(myProblem->getDimension());
    for(int i=0;i<myProblem->getDimension();i++)
    {
        double delta=rmargin[i]-lmargin[i];
        double left=lmargin[i]+x[i*2+0]*delta;
        double right=rmargin[i]-x[i*2+1]*delta;

        if(left>right)
        {
            double t=left;
            left=right;
            right=left;
        }
        leftArray[i]=left;
        rightArray[i]=right;
    }
}

double     IntervalMethod::fitness(Data &x)
{
    double average=0.0;
    Data trialx;
    trialx.resize(myProblem->getDimension());
    int nsamples=params["interval_samples"].toString().toInt();
    Data leftArray;
    Data rightArray;
    getInterval(x,leftArray,rightArray);
    for(int i=0;i<nsamples;i++)
    {
        for(int j=0;j<myProblem->getDimension();j++)
        {

            double newDelta=rightArray[j]-leftArray[j];
            trialx[j]=leftArray[j]+randNumber[i][j]*newDelta;
            if(trialx[j]<lmargin[j]) return 1e+100;
            if(trialx[j]>rmargin[j]) return 1e+100;
        }
        Tolmin mTolmin(myProblem,leftArray,rightArray);
        double f=mTolmin.Solve(trialx,10);
       if(i==0 || f<average){
           bestx=trialx;
           average=f;
    }
    }
    return average;
}

void    IntervalMethod::done()
{
    Data leftArray;
    Data rightArray;
    getInterval(chromosome[0],leftArray,rightArray);
    fitness(chromosome[0]);
    Tolmin mTolmin(myProblem,leftArray,rightArray);
    fitness_array[0]=mTolmin.Solve(bestx);
}

void    IntervalMethod::calcFitnessArray()
{
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<genome_count;i++)
    {
        fitness_array[i]=fitness(chromosome[i]);
    }
}

void    IntervalMethod::select()
{
    Data itemp;
    itemp.resize(genome_size);
    for(int i=0;i<genome_count;i++)
    {
        for(int j=0;j<genome_count-1;j++)
        {
            if(fitness_array[j+1]<fitness_array[j])
            {
                double dtemp;
                dtemp=fitness_array[j];
                fitness_array[j]=fitness_array[j+1];
                fitness_array[j+1]=dtemp;

                itemp=chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=itemp;
            }
        }
    }
}

void    IntervalMethod::crossover()
{
    double selection_rate=params["interval_selection_rate"].toString().toDouble();
    int nchildren=(int)((1.0 - selection_rate) * genome_count);
    if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    while(1)
    {
        tournament(parent0,parent1);
        for(int i=0;i<genome_size;i++)
        {
            double alfa,b,u,g1,g2;
            double x1,x2;
            int p1,p2;
            x1=parent0[i];
            x2=parent1[i];
            alfa=myProblem->randomDouble();
            if(alfa<0.5)
            {
                g1=x1;
                g2=x2;
            }
            else
            {
                g2=x1;
                g1=x2;
            }
            children[count_children][i]=g1;
            children[count_children+1][i]=g2;
        }
        count_children+=2;
        if(count_children>=nchildren) break;
     }
    for(int i=0;i<nchildren;i++)
    {
        chromosome[genome_count-i-1]=children[i];
    }
}

void    IntervalMethod::mutate()
{
    int start = 1;
    double mutation_rate=params["interval_mutation_rate"].toString().toDouble();
    for(int i=start;i<genome_count;i++)
    {
        for(int j=0;j<genome_size;j++)
        {
            double r=myProblem->randomDouble();
            if(r<mutation_rate)
            {
                    chromosome[i][j]=myProblem->randomDouble();
            }
        }
    }
}

void    IntervalMethod::getTournamentElement(Data &x)
{
    const int tournament_size =(genome_count<=100)?4:10;
    double max_fitness=1e+100;
    int    max_index=-1;
        for(int j=0;j<tournament_size;j++)
        {
        int r=rand() % (genome_count);
                if(j==0 || fitness_array[r]<max_fitness)
                {
                        max_index=r;
                        max_fitness=fitness_array[r];
                }
        }
    x=chromosome[max_index];
}

void    IntervalMethod::tournament(Data &p1,Data &p2)
{
    getTournamentElement(p1);
    getTournamentElement(p2);
}

IntervalMethod::~IntervalMethod()
{

}


extern "C" INTERVALMETHOD_EXPORT Optimizer *createOptimizer(Problem *p)
{
     return new IntervalMethod(p);
}

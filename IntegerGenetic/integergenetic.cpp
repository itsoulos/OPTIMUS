#include "integergenetic.h"


IntegerGenetic::IntegerGenetic(Problem *p)
    :Optimizer(p)
{
    addParameter("integer_chromosomes","500","Number of chromosomes");
    addParameter("integer_generations","500","Number of generations");
    addParameter("integer_selectionrate","0.10","Selection rate");
    addParameter("integer_mutationrate","0.05","Mutation rate");
    addParameter("integer_localsearchrate","0.01","Local search rate");
    addParameter("integer_stoprule","doublebox","Stopping rule (doublebox,generations,stoponzero)");

}

bool    IntegerGenetic::terminated()
{

    int max_generations=params["integer_generations"].toString().toInt();
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

     QString rule = params["integer_stoprule"].toString();

     if(rule=="doublebox" && stopat<1e-8 && generation>=10) return true;
     printf("Genetic. Generation: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n",
            generation,fitness_array[0],variance,stopat);
    if(rule == "doublebox")
     return generation>=max_generations|| (variance<=stopat && generation>=20);
    else
    if(rule == "generations")
        return generation >= max_generations;
    else
    if(rule == "stoponzero")
        return generation >= max_generations || fabs(fitness_array[0])<1e-8;
    return false;
}

void    IntegerGenetic::step()
{
    if(generation) mutate();
    calcFitnessArray();
    select();
    crossover();
 /*  if(generation %10==0)
    {
        for(int i=0;i<50;i++)
            randomSearch(rand() % chromosome.size());
        select();
    }*/
    ++generation;
}

Data    IntegerGenetic::fromIDATA(IDATA x)
{
    Data tx;
    tx.resize(x.size());
    for(int i=0;i<(int)x.size();i++)
        tx[i]=x[i];
    return tx;
}

IDATA   IntegerGenetic::toIDATA(Data x)
{
    IDATA tx;
    tx.resize(x.size());
    for(int i=0;i<(int)x.size();i++)
    {

        tx[i]=(int)x[i];
    }
    return tx;
}

void    IntegerGenetic::init()
{
    generation = 0;
    int count = params["integer_chromosomes"].toString().toInt();
    chromosome.resize(count);
    children.resize(count);
    fitness_array.resize(count);
    for(int i=0;i<count;i++)
    {
        chromosome[i].resize(myProblem->getDimension());
        children[i].resize(myProblem->getDimension());
        chromosome[i]=toIDATA(myProblem->getRandomPoint());
        Data tx = fromIDATA(chromosome[i]);
        fitness_array[i]=myProblem->funmin(tx);
    }
    lmargin=myProblem->getLeftMargin();
    rmargin=myProblem->getRightMargin();

    x1=0.0;
    x2=0.0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;
}

void    IntegerGenetic::done()
{
    Data tx = fromIDATA(chromosome[0]);
    fitness_array[0]=localSearch(tx);
}

void    IntegerGenetic::calcFitnessArray()
{
    int count = params["integer_chromosomes"].toString().toInt();

    double rate = params["integer_localsearchrate"].toString().toDouble();
    double dmin = 1e+100;
    for(int i=0;i<count;i++)
    {
        Data tx = fromIDATA(chromosome[i]);
        fitness_array[i]=myProblem->funmin(tx);
        if(rate>0 && myProblem->randomDouble()<rate)
        {

            double df=localSearch(tx);
            if(df<=fitness_array[i])
            {
                fitness_array[i]=myProblem->funmin(tx);
                chromosome[i]=toIDATA(tx);
            }
		
        }
	if(fabs(fitness_array[i])<dmin) dmin = fabs(fitness_array[i]);
	if(i%20==0){ printf("%d:%lf ",i,dmin); fflush(stdout);}
    }
    printf("\n");
}

void    IntegerGenetic::select()
{
    IDATA itemp;
    itemp.resize(myProblem->getDimension());
    int count = params["integer_chromosomes"].toString().toInt();

    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count-1;j++)
        {
            if(fitness_array[j+1]<fitness_array[j])
            {
                itemp = chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=itemp;
                double dtemp = fitness_array[j];
                fitness_array[j]=fitness_array[j+1];
                fitness_array[j+1]=dtemp;
            }
        }
    }
}

int		IntegerGenetic::randomChromosome()
{
    double r = myProblem->randomDouble();
    int count = params["integer_chromosomes"].toString().toInt();

    int pos = r * count;
    if(pos == count) pos --;
    return pos;
}

void    IntegerGenetic::onepointCrossover(IDATA &parent1, IDATA &parent2, IDATA &children1, IDATA &children2)
{
    int chromosome_size=myProblem->getDimension();
    int cutPoint=(int)(myProblem->randomDouble()*chromosome_size);
    std::copy(parent1.begin(),parent1.begin()+cutPoint,children1.begin());
    std::copy(parent2.begin()+cutPoint,parent2.end(),children1.begin()+cutPoint);
    std::copy(parent2.begin(),parent2.begin()+cutPoint,children2.begin());
    std::copy(parent1.begin()+cutPoint,parent1.end(),children2.begin()+cutPoint);
}

void    IntegerGenetic::crossover()
{
    int count = params["integer_chromosomes"].toString().toInt();

    double selection_rate=params["integer_selectionrate"].toString().toDouble();
    int nchildren=(int)((1.0 - selection_rate) * count);
    if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    IDATA parent0,parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    while(1)
    {
        tournament(parent0,parent1);
        onepointCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        count_children+=2;
        if(count_children>=nchildren) break;
    }
    for(int i=0;i<nchildren;i++)
    {
        chromosome[count-i-1]=children[i];
    }
}

void    IntegerGenetic::mutate()
{
    int start = 1;
    double mutation_rate=params["integer_mutationrate"].toString().toDouble();
    int count = params["integer_chromosomes"].toString().toInt();

    for(int i=start;i<count;i++)
    {
        for(int j=0;j<myProblem->getDimension();j++)
        {
            double r=myProblem->randomDouble();
            if(r<mutation_rate)
            {
                double px = myProblem->randomDouble();
                chromosome[i][j]=(int)(lmargin[j]+px * (rmargin[j]-lmargin[j]));
            }
        }
    }
}

void    IntegerGenetic::randomSearch(int pos)
{
    int size=chromosome[0].size();
        int count=chromosome.size();
        vector<int> tempx;
        tempx.resize(size);
        for(int iters=1;iters<=100;iters++)
        {
           int gpos=rand() % count;
           int cutpoint=rand() % size;
           for(int j=0;j<cutpoint;j++)    tempx[j]=chromosome[pos][j];
           for(int j=cutpoint;j<size;j++) tempx[j]=chromosome[gpos][j];
           Data tx = fromIDATA(tempx);
           double f=myProblem->funmin(tx);
           if(fabs(f)<fabs(fitness_array[pos]))
           {
               for(int j=0;j<size;j++) chromosome[pos][j]=tempx[j];
               fitness_array[pos]=f;
               break;
            }
            else
            {
              for(int j=0;j<cutpoint;j++) tempx[j]=chromosome[gpos][j];
              for(int j=cutpoint;j<size;j++) tempx[j]=chromosome[pos][j];
              Data tx = fromIDATA(tempx);
              double f=myProblem->funmin(tx);
              if(fabs(f)<fabs(fitness_array[pos]))
              {
                 for(int j=0;j<size;j++) chromosome[pos][j]=tempx[j];
                 fitness_array[pos]=f;
                 break;
                }
             }
            }
}

void	IntegerGenetic::getTournamentElement(IDATA &x)
{
    int count = params["integer_chromosomes"].toString().toInt();

    const int tournament_size =(count<=100)?4:10;
     double max_fitness=1e+100;
     int    max_index=-1;
         for(int j=0;j<tournament_size;j++)
         {
         int r= randomChromosome();
                 if(j==0 || fitness_array[r]<max_fitness)
                 {
                         max_index=r;
                         max_fitness=fitness_array[r];
                 }
         }
     x=chromosome[max_index];
}

void	IntegerGenetic::tournament(IDATA &p1,IDATA &p2)
{
    getTournamentElement(p1);
    getTournamentElement(p2);
}

void    IntegerGenetic::Solve()
{
    Optimizer::Solve();
}

IntegerGenetic::~IntegerGenetic()
{

}

extern "C"  INTEGERGENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new IntegerGenetic(p);
}

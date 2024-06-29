#include "doublegenetic.h"


DoubleGenetic::DoubleGenetic(Problem *p)
    :Optimizer(p)
{
    addParameter("double_chromosomes","100","Number of chromosomes");
    addParameter("double_generations","200","Number of generations");
    addParameter("double_selectionrate","0.10","Selection rate");
    addParameter("double_mutationrate","0.05","Mutation rate");
    addParameter("double_localsearchrate","0.00","Local search rate");
    addParameter("double_stoprule","doublebox","Stopping rule (doublebox,generations,stoponzero)");
    addParameter("double_debug","no","If it yes the full debug will be printed");
}

void    DoubleGenetic::calcFitnessArray()
{
    double rate = params["double_localsearchrate"].toString().toDouble();
    double dmin=1e+100;
    QString debug = params["double_debug"].toString();
    for(int i=0;i<(int)chromosome.size();i++)
    {
        fitnessArray[i]=myProblem->funmin(chromosome[i]);
        if(rate>0 && myProblem->randomDouble()<=rate)
        {
            fitnessArray[i]=localSearch(chromosome[i]);
        }
        if(fitnessArray[i]<dmin) dmin=fitnessArray[i];
        if(debug=="yes" && i%10==0)
        {
            printf("%d:%20.12lg ",i,dmin);
            fflush(stdout);
        }
    }
    if(debug == "yes")
        printf("\n");
}

void    DoubleGenetic::select()
{
    Data itemp;
    itemp.resize(myProblem->getDimension());
    int count = params["double_chromosomes"].toString().toInt();

    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count-1;j++)
        {
            if(fitnessArray[j+1]<fitnessArray[j])
            {
                itemp = chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=itemp;
                double dtemp = fitnessArray[j];
                fitnessArray[j]=fitnessArray[j+1];
                fitnessArray[j+1]=dtemp;
            }
        }
    }
}
void    DoubleGenetic::doubleCrossover(Data &parent1, Data &parent2,
                                       Data &children1, Data &children2)
{
    for(int i=0;i<(int)parent1.size();i++)
        {
            double x1=parent1[i];
            double x2=parent2[i];
            double alfa=-0.5+2.0*myProblem->randomDouble();
            double g1=alfa*x1+(1.0-alfa)*x2;
            double g2=alfa*x2+(1.0-alfa)*x1;
            if(g1>rmargin[i] || g1<lmargin[i])  g1=x1;
            if(g2>rmargin[i] || g2<lmargin[i])  g2=x2;
            children1[i]=g1;
            children2[i]=g2;
        }
}
void    DoubleGenetic::crossover()
{
    int count = params["double_chromosomes"].toString().toInt();
    double selection_rate=params["double_selectionrate"].toString().toDouble();
    int nchildren=(int)((1.0 - selection_rate) * count);
    if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    Data parent0,parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    while(1)
    {
        tournament(parent0,parent1);
        doubleCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        count_children+=2;
        if(count_children>=nchildren) break;
    }
    for(int i=0;i<nchildren;i++)
    {
        chromosome[count-i-1]=children[i];
    }
}

void    DoubleGenetic::mutate()
{
    int istart = 1;
    int count = params["double_chromosomes"].toString().toInt();
    double mutation_rate=params["double_mutationrate"].toString().toDouble();

    for(int i=istart;i<count;i++)
    {
        for(int j=0;j<myProblem->getDimension();j++)
        {
            double r = myProblem->randomDouble();
            if(r<=mutation_rate)
            {
                chromosome[i][j]=lmargin[j]+myProblem->randomDouble()*(rmargin[j]-lmargin[j]);
            }
        }
    }
}

void	DoubleGenetic::getTournamentElement(Data &x)
{
    int count = params["double_chromosomes"].toString().toInt();

    const int tournament_size =(count<=100)?4:10;
     double max_fitness=1e+100;
     int    max_index=-1;
         for(int j=0;j<tournament_size;j++)
         {
         int r= randomChromosome();
                 if(j==0 || fitnessArray[r]<max_fitness)
                 {
                         max_index=r;
                         max_fitness=fitnessArray[r];
                 }
         }
     x=chromosome[max_index];
}

void	DoubleGenetic::tournament(Data &p1,Data &p2)
{
    getTournamentElement(p1);
    getTournamentElement(p2);
}

int		DoubleGenetic::randomChromosome()
{
    double r = myProblem->randomDouble();
    int count = params["double_chromosomes"].toString().toInt();

    int pos = r * count;
    if(pos == count) pos --;
    return pos;
}


bool    DoubleGenetic::terminated()
{
    int max_generations=params["double_generations"].toString().toInt();
       double fmin=fabs(1.0+fabs(fitnessArray[0]));

       x1=x1+fmin;
       x2=x2+fmin * fmin;


       variance = x2/(generation+1) -(x1/(generation+1))*(x1/(generation+1));
       variance=fabs(variance);

       if(fitnessArray[0]<oldBesty)
       {
           oldBesty=fitnessArray[0];

           stopat=variance/2.0;
       }

       QString rule = params["double_stoprule"].toString();
       if(rule=="doublebox" && stopat<1e-8 && generation>=10) return true;
       //printf("Genetic. Generation: %4d Fitness: %20.12lf Variance: %10.5lf Stopat: %10.5lf \n",generation,fitnessArray[0],variance,stopat);

       if(rule == "doublebox")
       return generation>=max_generations|| (variance<=stopat && generation>=20);
       else
       if(rule=="generations")
           return generation>=max_generations;
       else
       if(rule=="stoponzero")
           return generation>=max_generations || fabs(fitnessArray[0])<1e-8;
       return false;
}

void    DoubleGenetic::step()
{
    if(generation) mutate();
    calcFitnessArray();
    select();
    crossover();
    ++generation;
}

void    DoubleGenetic::init()
{
    int count = params["double_chromosomes"].toString().toInt();
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();

    sampleFromProblem(count,chromosome,fitnessArray);
    children.resize(count);
    for(int i=0;i<count;i++)
    {
        children[i].resize(myProblem->getDimension());
    }
    x1=0.0;
    x2=0.0;
    oldBesty=1e+100;
    generation = 0;
}

void    DoubleGenetic::done()
{
    fitnessArray[0]=localSearch(chromosome[0]);
}

void    DoubleGenetic::Solve()
{
    Optimizer::Solve();
}

DoubleGenetic::~DoubleGenetic()
{

}

extern "C"  DOUBLEGENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new DoubleGenetic(p);
}

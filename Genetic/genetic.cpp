#include "genetic.h"
# define DOUBLE_CROSSOVER
# define DOUBLE_MUTATION
Genetic::Genetic(Problem *p)
    :Optimizer(p)
{
    addParameter("chromosome_count","200","Number of chromosomes");
    addParameter("max_generations","200","Maximum number of generations");
    addParameter("selection_rate","0.1","Selection rate");
    addParameter("mutation_rate","0.05","Mutation rate");
    addParameter("localsearch_rate","0.0","Local search rate");
}

void Genetic::setSettings(QJsonObject settings)
{
    int gcount=settings["chromosome_count"].toString().toInt(),
            maxg=settings["max_generations"].toString().toInt();


    addParameter("chromosome_count",QString::number(gcount),"Number of chromosomes");
    addParameter("max_generations",QString::number(maxg),"Maximum number of generations");
    addParameter("selection_rate",settings["selection_rate"].toString(),"Selection rate");
    addParameter("mutation_rate",settings["mutation_rate"].toString(),"Mutation rate");
    addParameter("localsearch_rate",settings["localsearch_rate"].toString(),"Local search rate");
    chromosome.resize(gcount);
    fitness_array.resize(gcount);
    children.resize(gcount);
    int gsize=myProblem->getDimension();
    for(int i=0;i<gcount;i++)
    {
        chromosome[i].resize(gsize);
        children[i].resize(gsize);
        chromosome[i]=myProblem->getRandomPoint();
        fitness_array[i]=fitness(chromosome[i]);
    }
    lmargin=myProblem->getLeftMargin();
    rmargin=myProblem->getRightMargin();
    parent0.resize(gsize);
    parent1.resize(gsize);
}


Genetic::~Genetic()
{


}
double     Genetic::fitness(Data &x)
{
    if(!myProblem->isPointIn(x)) return 1e+100;
    return myProblem->funmin(x);
}

bool       Genetic::terminated()
{
    int max_generations=params["max_generations"].toString().toInt();
    double fmin=fabs(1.0+fabs(fitness_array[0]));

    x1=x1+fmin;
    x2=x2+fmin * fmin;


    variance = x2/(generation+1) -(x1/(generation+1))*(x1/(generation+1));
    variance=fabs(variance);

    if(fitness_array[0]<oldBesty)
    {
        oldBesty=fitness_array[0];

        stopat=variance/2.0;
        printf("Changing values %lf -> %lf \n",variance,stopat);
    }

    if(stopat<1e-8 && generation>=20) return true;
    printf("Generation fit: %d value: %lf variance: %lf stopat: %lf\n",
           generation,
            fitness_array[0],variance,stopat);
    return generation>=max_generations || (variance<=stopat && generation>=20);
}


bool  Genetic::checkForGradientCriterion(Data &x)
{
    double dmin=1e+100;
    int imin=0;
    if(minimax.size()>0)
    {
    dmin=getDistance(minimax[0],x);
    for(int j=0;j<minimax.size();j++)
    {
        double d=getDistance(minimax[j],x);
        if(d<dmin)
        {
            imin=j;
            dmin=d;
        }
    }
    }
    else return false;
    if(dmin<1e-6 || (dmin<RC/localSearchCount && myProblem->getGradientCriterion(x,minimax[imin])))
        return true;
    return false;
}

void    Genetic::calcFitnessArray()
{
    int genome_count = chromosome.size();
    double localsearch_rate = params["localsearch_rate"].toString().toDouble();
    double dmin=1e+100;

    QVector<double> randomNums;
    for(int i=0;i<genome_count;i++)
        randomNums<<myProblem->randomDouble();

//#pragma omp parallel for reduction(+:RC)
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<genome_count;i++)
    {

        if(localsearch_rate>0 && randomNums[i]<=localsearch_rate)
        {
            if(checkForGradientCriterion(chromosome[i]))
            {
                fitness_array[i]=fitness(chromosome[i]);
                continue;
            }
            Data dg=chromosome[i];

            Tolmin mTolmin(myProblem);
            fitness_array[i]=mTolmin.Solve(chromosome[i]);

            RC+=getDistance(chromosome[i],dg);
            localSearchCount++;
#pragma omp critical
{
            bool found=false;
            for(int j=0;j<minimax.size();j++)
            {
                if(getDistance(minimax[j],chromosome[i])<1e-5) {found=true;break;}
            }
            if(!found)

            {

            minimax.push_back(chromosome[i]);
            }
            minimax.push_back(dg);

            }
        }
        else
        {
        fitness_array[i]=fitness(chromosome[i]);
        }
    }
}

void    Genetic::select()
{
    int genome_size=myProblem->getDimension();
    int genome_count=chromosome.size();
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


void	Genetic::getTournamentElement(Data &x)
{
    int genome_count=chromosome.size();
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

void	Genetic::tournament(Data &p1,Data &p2)
{
    getTournamentElement(p1);
    getTournamentElement(p2);
}


void    Genetic::crossover()
{
    double selection_rate=params["selection_rate"].toString().toDouble();
    int genome_count=chromosome.size();
    int nchildren=(int)((1.0 - selection_rate) * genome_count);
if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    int genome_size=myProblem->getDimension();
int total_success=0;
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
#ifdef LAPLACE_CROSSOVER
    u=problem->randomDouble();
    alfa=0.0;
    if(u<=0.5) b=alfa-b*log(u); else b=alfa+b*log(u);
    g1=x1+b*fabs(x1-x2);
    g2=x2+b*fabs(x1-x2);
#endif
#ifdef DOUBLE_CROSSOVER
alfa=-0.5+2.0*myProblem->randomDouble();
g1=alfa*x1+(1.0-alfa)*x2;
g2=alfa*x2+(1.0-alfa)*x1;
#endif
#ifdef UNIFORM_CROSSOVER
    alfa=problem->randomDouble();
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
#endif
    if(g1>rmargin[i] || g1<lmargin[i])  g1=x1;
    if(g2>rmargin[i] || g2<lmargin[i])  g2=x2;

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

void    Genetic::mutate()
{
    int start = 1;
    int maxGenerations=params["max_generations"].toString().toDouble();
    const double b=(1.0-generation*1.0/maxGenerations)*5.0;
    int genome_count=chromosome.size();
    double mutation_rate=params["mutation_rate"].toString().toDouble();
    int genome_size=myProblem->getDimension();
    for(int i=start;i<genome_count;i++)
    {
        for(int j=0;j<genome_size;j++)
        {
            double r=myProblem->randomDouble();
            if(r<mutation_rate)
            {
                double old=chromosome[i][j];
#ifdef MPT_MUTATION
                double t,t2,rk,beta=5.0;
                t=(genome[i][j]-lmargin[j])/(rmargin[j]-genome[i][j]);
                do
                {
                    rk=problem->randomDouble();
                    if(rk<t) t2=t-t*pow((t-rk)/t,beta);
                    else
                    if(rk==t) t2=t;
                    else
                    t2=t+(1.0-t)*pow((rk-t)/(1.0-t),beta);
                    genome[i][j]=(1.0-t2)*lmargin[j]+t2*rmargin[j];
                }while(genome[i][j]<lmargin[j] || genome[i][j]>rmargin[j]);
#endif
#ifdef PSO_MUTATION
                do{
                genome[i][j]=problem->randomDouble()*(bestgenome[i][j]-genome[i][j])+drand48()*(genome[0][j]-genome[i][j]);
                }while(genome[i][j]<lmargin[j]||genome[i][j]>rmargin[j]);
#endif
#ifdef RANDOM_MUTATION
                do{
                    genome[i][j]*=2.0*problem->randomDouble()-1.0;
                }while(genome[i][j]<lmargin[j]||genome[i][j]>rmargin[j]);
#endif
#ifdef DOUBLE_MUTATION
                double xi,yi;
                do{
                    xi=myProblem->randomDouble();
                    yi=myProblem->randomDouble();
                if(xi>0.5)
                {
                chromosome[i][j]=chromosome[i][j]+(rmargin[j]-chromosome[i][j])*(1.0-pow(yi,b));
                }
                else
                {
                chromosome[i][j]=chromosome[i][j]-(chromosome[i][j]-lmargin[j])*(1.0-pow(yi,b));
                }
                }while(chromosome[i][j]<lmargin[j]
                    || chromosome[i][j]>rmargin[j]);
#endif
            if(chromosome[i][j]<lmargin[j]||chromosome[i][j]>rmargin[j]) chromosome[i][j]=old;
            }
        }
    }
}

void       Genetic::step()
{
    if(generation) mutate();
    calcFitnessArray();
    select();
    crossover();
    ++generation;

}

void       Genetic::init()
{
    generation=0;
    x1=0.0;
    x2=0.0;
    variance=0.0;
    stopat=0.0;
    RC=0.0;
    oldBesty=1e+100;
    localSearchCount=0;
    minimax.resize(0);
    int gsize=myProblem->getDimension();
    int gcount=chromosome.size();
    for(int i=0;i<gcount;i++)
    {
        chromosome[i]=myProblem->getRandomPoint();
        fitness_array[i]=fitness(chromosome[i]);
    }
}

void       Genetic::done()
{
    Tolmin mTolmin(myProblem);
    fitness_array[0]=mTolmin.Solve(chromosome[0]);
}

void	Genetic::Solve()
{
	Optimizer::Solve();
}

extern "C"  GENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Genetic(p);
}


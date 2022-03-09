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
    addParameter("maxiters","1","Number of iterations of the algorithm"); 
    addParameter("genetic_crossover_type","double","The value used for crossover(double,uniform,laplace,onepoint");
    addParameter("genetic_mutation_type","double","The value  used for mutation (double,random,pso)");
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
    }

    if(stopat<1e-8 && generation>=10) return true;
//	printf("Genetic. Generation: %4d Fitness: %10.5lf Variance: %10.5lf Stopat: %10.5lf \n",generation,fitness_array[0],variance,stopat);
	
    return generation>=max_generations || (variance<=stopat && generation>=20);
}


bool  Genetic::checkForGradientCriterion(Data &x)
{
    double dmin=1e+100;
    int imin=0;
    vector<Data> localCopy;
#pragma omp critical
    localCopy=minimax;
    if(localCopy.size()>0)
    {
    dmin=getDistance(localCopy[0],x);
    for(int j=0;j<localCopy.size();j++)
    {
        double d=getDistance(localCopy[j],x);
        if(d<dmin)
        {
            imin=j;
            dmin=d;
        }
    }
    }
    else return false;
    if(dmin<1e-6 || (dmin<RC/localSearchCount && myProblem->getGradientCriterion(x,localCopy[imin])))
        return true;
    return false;
}

void    Genetic::calcFitnessArray()
{

    double localsearch_rate = params["localsearch_rate"].toString().toDouble();
    double dmin=1e+100;

    QVector<double> randomNums;
    for(int i=0;i<chromosome_count;i++)
        randomNums<<myProblem->randomDouble();

//#pragma omp parallel for reduction(+:RC)
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<chromosome_count;i++)
    {

        if(localsearch_rate>0 && randomNums[i]<=localsearch_rate)
        {
            if(checkForGradientCriterion(chromosome[i]))
            {
                fitness_array[i]=fitness(chromosome[i]);
                continue;
            }
            Data dg=chromosome[i];
            fitness_array[i]=localSearch(chromosome[i]);
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
    Data itemp;
    itemp.resize(chromosome_size);
    for(int i=0;i<chromosome_count;i++)
    {
        for(int j=0;j<chromosome_count-1;j++)
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
   const int tournament_size =(chromosome_count<=100)?4:10;
    double max_fitness=1e+100;
    int    max_index=-1;
        for(int j=0;j<tournament_size;j++)
        {
        int r=rand() % (chromosome_count);
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


void    Genetic::doubleCrossover(Data &parent1,Data &parent2,Data &children1,Data &children2)
{
    for(int i=0;i<parent1.size();i++)
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

void    Genetic::laplaceCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2)
{
    for(int i=0;i<parent1.size();i++)
    {
        double x1=parent1[i];
        double x2=parent2[i];
        double u=myProblem->randomDouble();
        double alfa=0.0;
        double b=0.0;
        if(u<=0.5) b=alfa-b*log(u); else b=alfa+b*log(u);
        double g1=x1+b*fabs(x1-x2);
        double g2=x2+b*fabs(x1-x2);
        if(g1>rmargin[i] || g1<lmargin[i])  g1=x1;
        if(g2>rmargin[i] || g2<lmargin[i])  g2=x2;
        children1[i]=g1;
        children2[i]=g2;
    }
}

void Genetic::uniformCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2)
{
    for(int i=0;i<parent1.size();i++)
    {
        double x1=parent1[i];
        double x2=parent2[i];
        double g1,g2;
        double alfa=myProblem->randomDouble();
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
        children1[i]=g1;
        children2[i]=g2;
    }
}

void    Genetic::onepointCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2)
{
    int cutPoint=(int)(myProblem->randomDouble()*chromosome_size);
    std::copy(parent1.begin(),parent1.begin()+cutPoint,children1.begin());
    std::copy(parent2.begin()+cutPoint,parent2.end(),children1.begin()+cutPoint);
    std::copy(parent2.begin(),parent2.begin()+cutPoint,children2.begin());
    std::copy(parent1.begin()+cutPoint,parent1.end(),children2.begin()+cutPoint);
}


void   Genetic::randomSearch(int pos)
{
    int size=chromosome[0].size();
        int count=chromosome.size();
        vector<double> tempx;
        tempx.resize(size);
        for(int iters=1;iters<=100;iters++)
        {
           int gpos=rand() % count;
           int cutpoint=rand() % size;
           for(int j=0;j<cutpoint;j++)    tempx[j]=chromosome[pos][j];
           for(int j=cutpoint;j<size;j++) tempx[j]=chromosome[gpos][j];
           double f=fitness(tempx);
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
              double f=fitness(tempx);
              if(fabs(f)<fabs(fitness_array[pos]))
              {
                 for(int j=0;j<size;j++) chromosome[pos][j]=tempx[j];
                 fitness_array[pos]=f;
                 break;
                }
             }
            }

}

void    Genetic::crossover()
{
    double selection_rate=params["selection_rate"].toString().toDouble();
    int nchildren=(int)((1.0 - selection_rate) * chromosome_count);
    if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    while(1)
    {
        tournament(parent0,parent1);
        if(crossover_type==CROSSOVER_DOUBLE)
        {
            doubleCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        }
        else
        if(crossover_type==CROSSOVER_LAPLACE)
        {
            laplaceCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        }
        else
        if(crossover_type==CROSSOVER_UNIFORM)
        {
            uniformCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        }
        else
        if(crossover_type==CROSSOVER_ONEPOINT)
        {
            onepointCrossover(parent0,parent1,children[count_children], children[count_children+1]);
        }
    count_children+=2;
    if(count_children>=nchildren) break;
    }
    for(int i=0;i<nchildren;i++)
    {
        chromosome[chromosome_count-i-1]=children[i];
    }
}


void    Genetic::mutate()
{
    int start = 1;
    int maxGenerations=params["max_generations"].toString().toDouble();
    const double b=(1.0-generation*1.0/maxGenerations)*5.0;
    double mutation_rate=params["mutation_rate"].toString().toDouble();
    for(int i=start;i<chromosome_count;i++)
    {
        for(int j=0;j<chromosome_size;j++)
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

	if(generation %10==0)
	{
		for(int i=0;i<20;i++)
			randomSearch(rand() % chromosome.size());
		select();
	}
}

void       Genetic::init()
{
    generation=0;
    x1=0.0;
    x2=0.0;
    variance=0.0;
    stopat=0.0;
    oldBesty=1e+100;

    chromosome_count=params["chromosome_count"].toString().toInt();
    chromosome_size=myProblem->getDimension();
    crossover_type=CROSSOVER_DOUBLE;
    mutation_type=MUTATION_DOUBLE;

    QString crossover_type_string=params["genetic_mutation_type"].toString();
    if(crossover_type_string=="double")
        crossover_type=CROSSOVER_DOUBLE;
    else
    if(crossover_type_string=="uniform")
        crossover_type=CROSSOVER_UNIFORM;
    else
    if(crossover_type_string=="laplace")
        crossover_type=CROSSOVER_LAPLACE;
    else
    if(crossover_type_string=="onepoint")
        crossover_type=CROSSOVER_ONEPOINT;

    if(chromosome.size()==0)
    {
        chromosome.resize(chromosome_count);
        fitness_array.resize(chromosome_count);
        children.resize(chromosome_count);
        int gsize=myProblem->getDimension();
        for(int i=0;i<chromosome_count;i++)
        {
            chromosome[i].resize(chromosome_size);
            children[i].resize(chromosome_size);
        }
        lmargin=myProblem->getLeftMargin();
        rmargin=myProblem->getRightMargin();
        parent0.resize(chromosome_size);
        parent1.resize(chromosome_size);
        iterNumber=1;
    }


    if(iterNumber==1)
    {
        localSearchCount=0;
        minimax.resize(0);
        RC=0.0;

        for(int i=0;i<chromosome_count;i++)
        {
            chromosome[i]=myProblem->getRandomPoint();
            fitness_array[i]=fitness(chromosome[i]);
        }
    }
}

void       Genetic::done()
{
    fitness_array[0]=localSearch(chromosome[0]);
}

void	Genetic::Solve()
{
    int maxIters=params["maxiters"].toString().toInt();
    if(maxIters==0)
    {
        iterNumber=1;
        Optimizer::Solve();
        return;
    }
    for(iterNumber=1;iterNumber<=maxIters;iterNumber++)
    {
        Optimizer::Solve();

    }
}

extern "C"  GENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Genetic(p);
}


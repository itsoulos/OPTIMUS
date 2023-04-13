#include "parallelgenetic.h"
ParallelGenetic::ParallelGenetic(Problem *p)
    :Optimizer(p)
{
    addParameter("pargen_count","200","Number of chromosomes");
    addParameter("pargen_iters","200","Number of iterations");
    addParameter("pargen_islands","1","Number of genetic islands");
    addParameter("pargen_srate","0.10","Selection rate");
    addParameter("pargen_mrate","0.05","Mutation rate");
    addParameter("pargen_migratecount","1","Number of migrated items");
    addParameter("pargen_tournamentsize","4","Size of tournament");
    addParameter("pargen_mutationpercent","0.05","The change percent in mutation");
    addParameter("pargen_migrategeneration","5","The number of iterations before migration");
    addParameter("pargen_termination","master","Parallel termination rule. Available values: master,one,majority");
    addParameter("pargen_migrationmethod","1to1","Migration method");
    addParameter("pargen_termmethod","doublebox","The termination method.");
    addParameter("pargen_similaritycount","5","The number of generations for similarity stopping rule");
    addParameter("pargen_debug","yes","Display pargen messages");
}

void    ParallelGenetic::calcFitnessArray(int islandIndex)
{
  int spos = subClusterStartPos(islandIndex);
  int epos = subClusterEndPos(islandIndex);
  for(int i=spos;i<=epos;i++)
  {
	  double r = myProblem->randomDouble();
	  if(r<0.01)
		  fitnessArray[i]=localSearch(chromosome[i]);
	  else
    	fitnessArray[i]=myProblem->funmin(chromosome[i]);
  }
}

void    ParallelGenetic::select(int islandIndex)
{
    int spos = subClusterStartPos(islandIndex);
    int epos = subClusterEndPos(islandIndex);
    Data xtemp;xtemp.resize(myProblem->getDimension());
    for(int i=spos;i<=epos;i++)
    {
        for(int j=spos;j<epos;j++)
        {
            if(fitnessArray[j+1]<fitnessArray[j])
            {
                double f= fitnessArray[j];
                fitnessArray[j]=fitnessArray[j+1];
                fitnessArray[j+1]=f;
                xtemp = chromosome[j];
                chromosome[j]=chromosome[j+1];
                chromosome[j+1]=xtemp;
            }
        }
    }
}

void    ParallelGenetic::crossover(int islandIndex)
{
    int nchildren=(int)((1.0 - pargen_srate) * pargen_count);
    if(!(nchildren%2==0)) nchildren++;
    int count_children=0;
    Data parent0,parent1;
    parent0.resize(myProblem->getDimension());
    parent1.resize(myProblem->getDimension());
    int spos = subClusterStartPos(islandIndex);
    int epos = subClusterEndPos(islandIndex);
    while(1)
    {
        tournament(islandIndex,parent0,parent1);
        doubleCrossover(parent0,parent1,
                        children[spos+count_children],
                children[spos+count_children+1]);
        count_children+=2;
        if(count_children>=nchildren) break;
    }
    for(int i=0;i<nchildren;i++)
    {

        chromosome[epos-i]=children[spos+i];
    }
}

void    ParallelGenetic::mutate(int islandIndex)
{
    int spos = subClusterStartPos(islandIndex)+1;
    int epos = subClusterEndPos(islandIndex);
    for(int i=spos;i<=epos;i++)
    {
        int gsize = chromosome[i].size();
        for(int j=0;j<gsize;j++)
        {
            double r = rand() *1.0/RAND_MAX;
            if(r<pargen_mrate)
            {
                double change = pargen_mutationpercent  * rand()*1.0/RAND_MAX;
                double delta = change * chromosome[i][j];
                double direction = rand()%2==1?1:-1;
                delta = delta * direction;
                chromosome[i][j]+=delta;

                if(chromosome[i][j]>rmargin[j])
                    chromosome[i][j]=rmargin[j];
                if(chromosome[i][j]<lmargin[j])
                    chromosome[i][j]=lmargin[j];

            }
        }
    }
}

void	ParallelGenetic::getTournamentElement(int islandIndex,Data &x)
{
    int pos = -1;
    double f = 1e+100;
 for(int i=0;i<pargen_tournamentsize;i++)
 {
     int rand_pos = randomChromosome(islandIndex);
     if(fitnessArray[rand_pos]<f)
     {
         f = fitnessArray[rand_pos];
         pos =  rand_pos;
     }
 }
 x = chromosome[pos];
}

void	ParallelGenetic::tournament(int islandIndex,Data &p1,Data &p2)
{
    getTournamentElement(islandIndex,p1);
    getTournamentElement(islandIndex,p2);
}

int		ParallelGenetic::randomChromosome(int islandIndex)
{
    int items = chromosome.size()/pargen_islands;
    int pos = rand() % items;
    return subClusterStartPos(islandIndex)+pos;
}

int     ParallelGenetic::subClusterStartPos(int islandIndex)
{
    return islandIndex * chromosome.size() / pargen_islands;

}

int     ParallelGenetic::subClusterEndPos(int islandIndex)
{
    return (islandIndex + 1) * chromosome.size() / pargen_islands - 1;

}

void    ParallelGenetic::doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2)
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


bool    ParallelGenetic::terminated()
{
    int spos = getBestIndex();
    if(pargen_debug=="yes")
    printf("ParGen. Generation: %4d BestFitness: %20.10lg\n",
           generation,fitnessArray[spos]);
   if(pargen_termination=="master")
   {
        return updateMasterTerminationRule();
   }
   else
   {
        int countTrue = 0;
        for(int t=0;t<pargen_islands;t++)
        {
            int t1 = updateTerminationRule(t);
            printf("true for island %d = > %d \n",t,t1);
            countTrue+=t1;
        }
        printf("generation: %d total true: %d \n",
               generation,
               countTrue);
        if(pargen_termination=="one")
            return countTrue>=1;
        else
        if(pargen_termination=="majority")
            return countTrue>=pargen_islands/2;
   }
   return false;
}

void    ParallelGenetic::migrateFromIslandtoIsland(int from,int to)
{
    select(from);
    select(to);
    int spos1 = subClusterStartPos(from);
    int epos2 = subClusterEndPos(to);
    for(int i=0;i<pargen_migratecount;i++)
    {
        if(fitnessArray[epos2-i]>fitnessArray[spos1+i])
        {
            chromosome[epos2-i]=chromosome[spos1+i];
            fitnessArray[epos2-i]=fitnessArray[spos1+i];
        }
    }
}

void    ParallelGenetic::migrate()
{

    if(pargen_islands<=1) return;

    if(pargen_migrationmethod=="1to1")
    {
        int t1 = rand() % pargen_islands;
        int t2;
        do
        {
            t2 = rand() % pargen_islands;
        }while(t1 == t2);
        migrateFromIslandtoIsland(t1,t2);
    }
    else
    if(pargen_migrationmethod=="1toN")
    {
        int t1 = rand() % pargen_islands;
        for(int t=0;t<pargen_islands;t++)
        {
            if(t==t1) continue;
            migrateFromIslandtoIsland(t1,t);
        }
    }
    else
    if(pargen_migrationmethod=="Nto1")
    {
        int t1 = rand() % pargen_islands;
        for(int t=0;t<pargen_islands;t++)
        {
            if(t==t1) continue;
            migrateFromIslandtoIsland(t,t1);
        }
    }
    else
    {
        for(int t1=0;t1<pargen_islands;t1++)
        {
            for(int t2=0;t2<t1;t2++)
                migrateFromIslandtoIsland(t1,t2);
        }
    }
}
void    ParallelGenetic::step()
{
    int t;
#pragma omp parallel for private(t) num_threads(pargen_islands)
    for (t = 0; t < pargen_islands; t++)
    {
        if(generation) mutate(t);
        calcFitnessArray(t);
        select(t);

        crossover(t);
    }
    ++generation;
      //migration check
    if(generation % pargen_migrategeneration==0)
    {
        migrate();

    }

}

void ParallelGenetic::init()
{
    /**
     * Get Parameters from the list**/
   pargen_count=params["pargen_count"].toString().toInt();
    pargen_iters=params["pargen_iters"].toString().toInt();
    pargen_islands=params["pargen_islands"].toString().toInt();//the number of parallel threads
    pargen_srate=params["pargen_srate"].toString().toDouble();//the selection rate
    pargen_mrate=params["pargen_mrate"].toString().toDouble();//the mutation rate
    pargen_migratecount=params["pargen_migratecount"].toString().toInt();
    pargen_tournamentsize=params["pargen_tournamentsize"].toString().toInt();
    pargen_mutationpercent=params["pargen_mutationpercent"].toString().toDouble();
    pargen_migrategeneration=params["pargen_migrategeneration"].toString().toInt();
    pargen_termination=params["pargen_termination"].toString();
    pargen_migrationmethod=params["pargen_migrationmethod"].toString();
    pargen_termmethod=params["pargen_termmethod"].toString();
    pargen_similaritycount=params["pargen_similaritycount"].toString().toInt();
    pargen_debug=params["pargen_debug"].toString();

    printf("migrate is %d \n",pargen_migratecount);
    printf("migration %s \n",pargen_migrationmethod.toStdString().c_str());
    chromosome.resize(pargen_count * pargen_islands);
    children.resize(pargen_count * pargen_islands);
    fitnessArray.resize(chromosome.size());
    for(int i=0;i<chromosome.size();i++)
    {
        chromosome[i].resize(myProblem->getDimension());
        children[i].resize(myProblem->getDimension());
        chromosome[i]=myProblem->getRandomPoint();
        fitnessArray[i]=myProblem->funmin(chromosome[i]);
    }
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();
    generation  = 0;

    xx1.resize(pargen_islands);
    xx2.resize(pargen_islands);
    stopat.resize(pargen_islands);
    variance.resize(pargen_islands);
    oldBesty.resize(pargen_islands);

    for(int t=0;t<pargen_islands;t++)
    {
        xx1[t]=0.0;
        xx2[t]=0.0;
        stopat[t]=0.0;
        variance[t]=0.0;
        oldBesty[t]=1e+100;
    }

    xx1_master=0.0;
    xx2_master=0.0;
    stopat_master=0.0;
    variance_master=0.0;
    oldBesty_master=1e+100;


    similarityValue.resize(pargen_islands);
    similarityCount.resize(pargen_islands);
    for(int i=0;i<pargen_islands;i++)
    {
        similarityValue[i]=1e+100;
        similarityCount[i]=0;
    }
    similarityValue_master=1e+100;
    similarityCount_master=0;
}

bool    ParallelGenetic::updateTerminationRule(int islandIndex)
{
    int spos = subClusterStartPos(islandIndex);

    if(pargen_termmethod=="doublebox")
    {

	    
    double fmin=fabs(fitnessArray[spos]-oldBesty[islandIndex]);
	if(generation<=1) fmin =fabs(fitnessArray[spos]);
	if(generation<=1) fmin=1;
	else fmin = fabs(fitnessArray[spos]-oldBesty[islandIndex])>1e-5;

    xx1[islandIndex]=xx1[islandIndex]+fmin;
    xx2[islandIndex]=xx2[islandIndex]+fmin * fmin;


    variance[islandIndex] = xx2[islandIndex]/(generation+1)
            -(xx1[islandIndex]/(generation+1))*(xx1[islandIndex]/(generation+1));
    variance[islandIndex]=fabs(variance[islandIndex]);

    if(fitnessArray[spos]<oldBesty[islandIndex])
    {
        oldBesty[islandIndex]=fitnessArray[spos];

        stopat[islandIndex]=variance[islandIndex]/2.0;
    }
    return (variance[islandIndex]<=stopat[islandIndex]&&generation>=5)
              || generation>=pargen_iters;
    }
    else
    {
        double ff = fitnessArray[spos];
        if(fabs(ff-similarityValue[islandIndex])<1e-5)
        {
            similarityCount[islandIndex]++;
            similarityValue[islandIndex]=ff;
        }
        else
        {
            similarityCount[islandIndex]=0;
            similarityValue[islandIndex]=ff;
        }
        return similarityCount[islandIndex]>=pargen_similaritycount
                || (generation>=pargen_iters);
    }
}

bool    ParallelGenetic::updateMasterTerminationRule()
{
    int spos = getBestIndex();

    if(pargen_termmethod=="doublebox")
    {
    double fmin=fabs(1.0+fabs(fitnessArray[spos]));
    if(generation<=1) fmin=1;
    else fmin=fabs(fitnessArray[spos]-oldBesty_master)>1e-5;

    xx1_master=xx1_master+fmin;
    xx2_master=xx2_master+fmin * fmin;


    variance_master = xx2_master/(generation+1)
            -(xx1_master/(generation+1))*(xx1_master/(generation+1));
    variance_master=fabs(variance_master);

    if(fitnessArray[spos]<oldBesty_master)
    {
        oldBesty_master=fitnessArray[spos];

        stopat_master=variance_master/2.0;
    }

    return (variance_master<=stopat_master &&generation>=5) ||
            generation >= pargen_iters;
    }
    else
    {
        if(fabs(similarityValue_master-fitnessArray[spos])<1e-5)
        {
            similarityCount_master++;
            similarityValue_master = fitnessArray[spos];
        }
        else
        {
            similarityCount_master=0;
            similarityValue_master = fitnessArray[spos];
        }
        return (similarityCount_master>=pargen_similaritycount)
            || (generation>=pargen_iters);
    }
}

int ParallelGenetic::getBestIndex()
{
    double f= 1e+100;
    int ipos = -1;
    for(int t=0;t<pargen_islands;t++)
    {
        int spos = subClusterStartPos(t);
        if(fitnessArray[spos]<f)
        {
            f = fitnessArray[spos];
            ipos = spos;
        }
    }
    return ipos;
}

void ParallelGenetic::done()
{
    int spos = getBestIndex();
    fitnessArray[spos]=localSearch(chromosome[spos]);

}



ParallelGenetic::~ParallelGenetic()
{
//nothing here
}


extern "C"  PARALLELGENETIC_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new ParallelGenetic(p);
}

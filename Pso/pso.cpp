#include "pso.h"

Pso::Pso(Problem *p)
    :Optimizer(p)
{
    addParameter("pso_particles","500","Number of pso particles");
    addParameter("pso_generations","500","Maximum number of pso generations");
    addParameter("pso_c1","1.0","Pso c1 parameter");
    addParameter("pso_c2","1.0","Pso c2 parameter");
    addParameter("pso_inertia_start","0.4","Start value for inertia");
    addParameter("pso_inertia_end","0.9","End value for inertia");
    addParameter("pso_localsearch_rate","0.0","Local search rate for pso");
}

bool Pso::checkGradientCriterion(Data &x)
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

bool Pso::terminated()
{
    int max_generations=params["pso_generations"].toString().toInt();
    double fmin=fabs(1.0+fabs(besty));
    if(generation<=1) {x1=0.0;x2=0.0;}
    x1+=fmin;
    x2+=fmin * fmin;
    variance = x2/(generation+1) -(x1/(generation+1))*(x1/(generation+1));
    variance=fabs(variance);
    if(besty<oldbesty)
    {
        oldbesty=besty;
        stopat=variance/2.0;
    }
    if(stopat<1e-8 && !isnan(variance)) stopat=variance/2.0;
    printf("Generation %d value: %lf variance: %lf stopat: %lf\n",generation,besty,
            variance,stopat);
    return generation>=max_generations;// || (variance<=stopat && generation>=20);
}

void Pso::step()
{
    ++generation;
    calcFitnessArray();
     updateBest();
}

void Pso::init()
{

    int pso_particles=params["pso_particles"].toString().toInt();
    particle.resize(pso_particles);
    bestParticle.resize(pso_particles);
    velocity.resize(pso_particles);
    fitness_array.resize(pso_particles);
    bestFitness_array.resize(pso_particles);
    bestx.resize(myProblem->getDimension());
    lmargin=myProblem->getLeftMargin();
    rmargin=myProblem->getRightMargin();


    generation=0;
    besty=1e+100;
    oldbesty=1e+100;
    x1=0.0;
    x2=0.0;
    stopat=0.0;
    variance=0.0;
    RC=0.0;
    localSearchCount=0;
    minimax.clear();

    for(int i=0;i<pso_particles;i++)
    {
        particle[i].resize(myProblem->getDimension());
        bestParticle[i].resize(myProblem->getDimension());
        velocity[i].resize(myProblem->getDimension());
        particle[i]=myProblem->getRandomPoint();
        bestParticle[i]=particle[i];
        fitness_array[i]=fitness(particle[i]);
        bestFitness_array[i]=fitness_array[i];

        for(int j=0;j<myProblem->getDimension();j++)
        {
          double left=-(rmargin[j]-lmargin[j])/20.0;
          double right=(rmargin[j]-lmargin[j])/20.0;
        velocity[i][j]=left+myProblem->randomDouble()*(right-left);
        }
    }
    updateBest();
}

void Pso::done()
{
    besty=localSearch(bestx);
}

void Pso::calcFitnessArray()
{
    int genome_count=params["pso_particles"].toString().toInt();
    double wmin=params["pso_inertia_start"].toString().toDouble();
    double wmax=params["pso_inertia_end"].toString().toDouble();
    int maxGenerations=params["pso_generations"].toString().toInt();
    Data distances;
    int genome_size=myProblem->getDimension();
    double inertia= wmax-generation*1.0/maxGenerations*(wmax-wmin);
    double c1=params["pso_c1"].toString().toDouble();
    double c2=params["pso_c2"].toString().toDouble();

    double localsearch_rate=params["pso_localsearch_rate"].toString().toDouble();
    for(int i=0;i<genome_count;i++)
    {
        Data oldg=particle[i];
        for(int j=0;j<genome_size;j++)
        {
            double r1=myProblem->randomDouble();
            double r2=myProblem->randomDouble();

            double tj=velocity[i][j];
            double part1=inertia * velocity[i][j];
            double part2=c1 * r1 * (bestParticle[i][j]-particle[i][j]);
            double part3=c2 * r2 * (bestx[j]-particle[i][j]);
            velocity[i][j]=part1+part2+part3;
            double trialf=particle[i][j]+velocity[i][j];
            if(trialf<lmargin[j] || trialf>rmargin[j])
            {
                velocity[i][j]=tj;
                continue;
            }
           else
            {
                particle[i][j]=trialf;

            }
        }
    distances.push_back(getDistance(particle[i],oldg));
    }
#pragma omp parallel for num_threads(threads)
    for(int i=0;i<genome_count;i++)
    {
        if(distances[i]>1e-6  )
        {
            if(localsearch_rate>0.0 && myProblem->randomDouble()<=localsearch_rate
                    && !checkGradientCriterion(particle[i]))
            {
                Data dg=particle[i];
                fitness_array[i]=localSearch(particle[i]);
                RC+=getDistance(dg,particle[i]);
                localSearchCount++;
                #pragma omp critical
                {
                    bool found=false;
                    for(int j=0;j<minimax.size();j++)
                    {
                        if(getDistance(minimax[j],particle[i])<1e-5) {found=true;break;}
                    }
                    if(!found)
                    {
                        minimax.push_back(particle[i]);
                    }
                    minimax.push_back(dg);
                }
            }
            else
            fitness_array[i]=fitness(particle[i]);
        }
    }
}

void Pso::updateBest()
{
    int genome_count=params["pso_particles"].toString().toInt();


    int mod=10;
    for(int i=0;i<genome_count;i++)
    {

        /*
        //if( fabs(fitness_array[i]-bestfitness_array[i])>1e-5  && rand()%mod==0)
        if( rand()%mod==0)
        {
            int imin=-1;
            double dmin=1e+100;
            for(int j=0;j<minimax.size();j++)
            {
                double d=getDistance(genome[i],minimax[j]);
                if(d<dmin)
                {
                    imin=j;
                    dmin=d;
                }
            }
            if(imin>=0 && dmin<RC/localSearchCount && problem->getGradientCriterion(genome[i],
                        minimax[imin]))
                ;
            else{

                bool notFoundBetter=true;

                if(notFoundBetter)
                {

            Data gg=genome[i];
            bool found=false;
            localSearch(genome[i],fitness_array[i]);
            for(int j=0;j<minimax.size();j++)
            {
                if(getDistance(genome[i],minimax[j])<1e-5) {found=true;break;}
            }
            if(!found)
            minimax.push_back(genome[i]);
            minimax.push_back(gg);
            RC+=getDistance(gg,genome[i]);
            localSearchCount++;
                }
                }
        }*/
        if(fitness_array[i]<bestFitness_array[i])
        {
            bestFitness_array[i]=fitness_array[i];
            bestParticle[i]=particle[i];
        }
        if(besty>bestFitness_array[i])
        {
            bestIndex=i;
            bestx=bestParticle[i];
            besty=bestFitness_array[i];
        }
    }

}

double  Pso::fitness(Data &x)
{
    if(!myProblem->isPointIn(x)) return 1e+100;
    return myProblem->funmin(x);
}

void Pso::Solve()
{
    Optimizer::Solve();
}

Pso::~Pso()
{

}

extern "C" PSO_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Pso(p);
}

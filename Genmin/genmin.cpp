#include "genmin.h"

Genmin::Genmin(Problem *p)
    :Optimizer(p)
{
    addParameter("genmin_chromosome_count","200","Number of chromosomes");
    addParameter("genmin_chromosome_size","100","Size of the chromosomes");
    addParameter("genmin_max_generations","200","Maximum number of generations");
    addParameter("genmin_selection_rate","0.1","Selection rate");
    addParameter("genmin_mutation_rate","0.05","Mutation rate");
}

bool Genmin::terminated()
{

    double f=-pop->getBestFitness();
    int iflag=0;

    if(f<initialy)
    {

        printf("New f is %lf\n",f);

        vector<int> genome;genome.resize(pop->getSize());
        Data trialx;
        trialx.resize(bestx.size());
        genome=pop->getBestGenome();
        program->getX(genome,trialx);

 //     program->getX(initialx,genome,trialx);
        double trialy=  localSearch(trialx);
        if(trialy<besty)
        {
           iflag=1;
           besty=trialy;
           bestx=trialx;
          //set back to population this value

        }
        initialy=f;

    }
    double v=fabs(besty);
    x1+=(v);
    x2+=(v)*(v);
    variance=x2/generation-(x1/generation)*(x1/generation);
    variance = fabs(variance);
    if(iflag || stopat<1e-7)
    {
            stopat=variance/2.0;
    }

    printf("Iter: %4d Value: %20.10lf Variance: %20.10lf Stopat: %20.10lf\n",
           generation,besty,variance,stopat);
    if(variance<=stopat && generation>=20) return true;
    if(generation>=maxGenerations) return true;
    return false;
}

void    Genmin::step()
{
    pop->nextGeneration();
    ++generation;
}

void    Genmin::init()
{
    int dimension = myProblem->getDimension();

    program = new RlsProgram(myProblem);

    initialx.resize(dimension);
    initialx=myProblem->getRandomPoint();
    genome_count=params["genmin_chromosome_count"].toString().toInt();
    genome_length =params["genmin_chromosome_size"].toString().toInt();
    selection_rate=params["genmin_selection_rate"].toString().toDouble();
    mutation_rate=params["genmin_mutation_rate"].toString().toDouble();
    maxGenerations=params["genmin_max_generations"].toString().toInt();
    pop =new MultiPopulation(genome_count,
                  genome_length ,dimension,program,initialx);
    pop->setSelectionRate(selection_rate);
    pop->setMutationRate(mutation_rate);
    bestx.resize(dimension);
    besty=1e+100;
    x1=0.0;
    x2=0.0;
    generation=1;
    bestx=initialx;
    besty=myProblem->funmin(bestx);
    initialy=besty;
}

void    Genmin::done()
{
    besty=localSearch(bestx);
}


Genmin::~Genmin()
{
    delete pop;
    delete program;
}

extern "C"  GENMIN_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new Genmin(p);
}


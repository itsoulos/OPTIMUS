#include "ipso.h"

iPso::iPso(Problem *p)
    : Optimizer(p)
{
    addParameter("ipso_particles", "100", "Number of pso particles");
    addParameter("ipso_generations", "100", "Maximum number of pso generations");
    addParameter("ipso_c1", "0.5", "Pso c1 parameter");
    addParameter("ipso_c2", "0.5", "Pso c2 parameter");
    addParameter("ipso_inertia_start", "0.4", "Start value for inertia");
    addParameter("ipso_inertia_end", "0.9", "End value for inertia");
    addParameter("ipso_localsearch_rate", "0.1", "Local search rate for pso");
}

bool iPso::checkGradientCriterion(Data &x)
{

    double dmin = 1e+100;
    int imin = 0;
    if (minimax.size() > 0)
    {
        dmin = getDistance(minimax[0], x);
        for (int j = 0; j < minimax.size(); j++)
        {
            double d = getDistance(minimax[j], x);
            if (d < dmin)
            {
                imin = j;
                dmin = d;
            }
        }
    }
    else
        return false;
    if (dmin < 1e-6 || (dmin < RC / localSearchCount && myProblem->getGradientCriterion(x, minimax[imin])))
        return true;
    return false;
}

bool iPso::terminated()
{
    int max_generations = params["ipso_generations"].toString().toInt();
    double fmin = fabs(1.0 + fabs(besty));
    if (generation <= 1)
    {
        x1 = 0.0;
        x2 = 0.0;
    }
    x1 += fmin;
    x2 += fmin * fmin;
    variance = x2 / (generation + 1) - (x1 / (generation + 1)) * (x1 / (generation + 1));
    variance = fabs(variance);
    if (besty < oldbesty)
    {
        oldbesty = besty;
        stopat = variance / 2.0;
    }
    if (stopat < 1e-8 && !isnan(variance))
        stopat = variance / 2.0;
    printf("Generations %d value: %lf variance: %lf stopat: %lf\n", generation, besty,
           variance, stopat);
    return generation >= max_generations; // || (variance<=stopat && generation>=20);
}

void iPso::step()
{
    ++generation;
    calcFitnessArray();
    updateBest();
}

void iPso::init()
{

    int ipso_particles = params["ipso_particles"].toString().toInt();
    particle.resize(ipso_particles);
    bestParticle.resize(ipso_particles);
    velocity.resize(ipso_particles);
    fitness_array.resize(ipso_particles);
    bestFitness_array.resize(ipso_particles);
    bestx.resize(myProblem->getDimension());
    lmargin = myProblem->getLeftMargin();
    rmargin = myProblem->getRightMargin();

    generation = 0;
    besty = 1e+100;
    oldbesty = 1e+100;
    x1 = 0.0;
    x2 = 0.0;
    stopat = 0.0;
    variance = 0.0;
    RC = 0.0;
    localSearchCount = 0;
    minimax.clear();

    for (int i = 0; i < ipso_particles; i++)
    {
        particle[i].resize(myProblem->getDimension());
        bestParticle[i].resize(myProblem->getDimension());
        velocity[i].resize(myProblem->getDimension());
        particle[i] = myProblem->getRandomPoint();
        bestParticle[i] = particle[i];
        fitness_array[i] = fitness(particle[i]);
        bestFitness_array[i] = fitness_array[i];

        for (int j = 0; j < myProblem->getDimension(); j++)
        {
            double left = -(rmargin[j] - lmargin[j]) / 20.0;
            double right = (rmargin[j] - lmargin[j]) / 20.0;
            velocity[i][j] = left + myProblem->randomDouble() * (right - left);
        }
    }
    updateBest();
}

void iPso::done()
{
    besty = localSearch(bestx);
}

void iPso::calcFitnessArray()
{
    int genome_count = params["ipso_particles"].toString().toInt();
    double wmin = params["ipso_inertia_start"].toString().toDouble();
    double wmax = params["ipso_inertia_end"].toString().toDouble();
    int maxGenerations = params["ipso_generations"].toString().toInt();
    Data distances;
    int genome_size = myProblem->getDimension();
    //inecria weight => εάν θα διατηρηθεί η ταχύτητα
    //double inertia = wmax - generation * 1.0 / maxGenerations * (wmax - wmin);
    double inertia = fabs((1.0/(2.0 - besty)));
    double c1 = params["ipso_c1"].toString().toDouble();
    double c2 = params["ipso_c2"].toString().toDouble();

    double localsearch_rate = params["ipso_localsearch_rate"].toString().toDouble();
    for (int i = 0; i < genome_count; i++)
    {
        Data oldg = particle[i];//αποθήκευση παλιάς θέσης
        for (int j = 0; j < genome_size; j++)
        {
            double r1 = myProblem->randomDouble();
            double r2 = myProblem->randomDouble();

            double tj = velocity[i][j];//αποθήκευση παλιάς ταχύτητας
            double part1 = inertia * velocity[i][j];//πολλαπλασιαστής ταχύτητας(inecria weight)
            double part2 = c1 * r1 * (bestParticle[i][j] - particle[i][j]);
            double part3 = c2 * r2 * (bestx[j] - particle[i][j]);
            velocity[i][j] = part1 + part2 + part3;// υπολογισμός ταχύτητας
            double trialf = particle[i][j] + velocity[i][j];//υπολογισμός θέσης
            if (trialf < lmargin[j] || trialf > rmargin[j])// εκτός ορίων
            {
                velocity[i][j] = tj;// ξαναβάζει την παλιά ταχύτητα
                continue;
            }
            else
            {
                particle[i][j] = trialf;// καινούργια θέση
            }
        }
        distances.push_back(getDistance(particle[i], oldg));// αποθήκευση της απόστασης => παλιά με καινούργια
    }

//#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < genome_count; i++)
    {
        if (distances[i] > 1e-6)
        {
            if (localsearch_rate > 0.0 && myProblem->randomDouble() <= localsearch_rate && !checkGradientCriterion(particle[i]))
            {
                Data dg = particle[i];
                fitness_array[i] = localSearch(particle[i]);
                RC += getDistance(dg, particle[i]);
                localSearchCount++;
//#pragma omp critical
                {
                    bool found = false;
                    for (int j = 0; j < minimax.size(); j++)
                    {
                        if (getDistance(minimax[j], particle[i]) < 1e-5)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        minimax.push_back(particle[i]);
                    }
                    minimax.push_back(dg);
                }
            }
            else
                fitness_array[i] = fitness(particle[i]);
        }
    }
}
//υπολογισμός όλων των καλύτερων θέσεων για όλα τα πουλάκια του σμήνους
void iPso::updateBest()
{
    int genome_count = params["ipso_particles"].toString().toInt();

    int mod = 10;

    for (int i = 0; i < genome_count; i++)
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
        if (fitness_array[i] < bestFitness_array[i])
        {
            bestFitness_array[i] = fitness_array[i];
            bestParticle[i] = particle[i];
        }
        if (besty > bestFitness_array[i])
        {
            bestIndex = i;
            bestx = bestParticle[i];
            besty = bestFitness_array[i];
        }
    }
}

double iPso::fitness(Data &x)
{
    if (!myProblem->isPointIn(x))
        return 1e+100;
    return myProblem->funmin(x);
}

void iPso::Solve()
{
    Optimizer::Solve();
}

iPso::~iPso()
{
}

extern "C" IPSO_EXPORT Optimizer *createOptimizer(Problem *p)
{
    return new iPso(p);
}

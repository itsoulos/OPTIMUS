#ifndef PDG_H
#define PDG_H

# include "pdg_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
# include "omp.h"
# include <time.h>
# include <QRandomGenerator>
# include <algorithm>
class pdgInterface
{
public:
    virtual ~pdgInterface()
    {

    }
private:

    virtual void	getTournamentElement(int subClusterIndex, Data &x)=0;
    virtual void	tournament(int subClusterIndex, Data &p1,Data &p2)=0;
public:

};

/**
 * @brief The DoubleGenetic class implements a Genetic Algorithm for
 * global optimization purposes.
 */
class PDG_EXPORT pdg: public Optimizer, pdgInterface
{
private:
    vector<vector<Data>> chromosome;
    vector<vector<Data>> children;
    vector<Data> fitnessArray;

    Data lmargin,rmargin, chrom;;
    double localRate;

    QString stopRule,propagateMethod, debug;
    double selectionRate;
    double mutationRate;

    int subCluster, subClusterEnable, pNumber, similarityMaxCount, populationOfCluster, propagateRate, generations, subEnable;
    vector<double> sum, newSum, MO, newMO, bestF2xInCluster, bestF2xInClusterOLD, bestx,doublebox_xx1 ,doublebox_xx2,doublebox_variance,doublebox_stopat, oldBesty;
    vector<int> similarityCurrentCount, generation;
    std::chrono::time_point<std::chrono::system_clock> before, after;

    void	getTournamentElement(int subClusterIndex, Data &x);
    void	tournament(int subClusterIndex, Data &p1,Data &p2);
    int		randomChromosome();
    void    doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2);

    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void replace(int subClusterIndex, vector<pair<double, Data>> chrom);
    void propagate();
    void calcFitnessArray(int subClusterIndex);
    void select(int subClusterIndex);
    void mutate(int subClusterIndex);
    void crossover(int subClusterIndex);
    bool checkSubCluster(int subClusterName);

public:

    pdg(Problem *p);
    virtual         void Solve();
    virtual         ~pdg();
};

extern "C" PDG_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PDGGENETIC_H

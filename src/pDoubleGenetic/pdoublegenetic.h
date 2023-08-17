#ifndef PDOUBLEGENETIC_H
#define PDOUBLEGENETIC_H

#include "pDoubleGenetic_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>
# include <Optimus/tolmin.h>
# include <Optimus/gradientdescent.h>
#include <Optimus/kmeans.h>
#include <Optimus/cluster.h>
#include <Optimus/point.h>
# include "omp.h"
#include <time.h>
#include <QRandomGenerator>
class pDoubleGeneticInterface
{
public:
    virtual ~pDoubleGeneticInterface()
    {

    }
private:
    virtual void	getTournamentElement(int subCluster, Data &x)=0;
    virtual void	tournament(int subCluster, Data &p1,Data &p2)=0;
public:

};

/**
 * @brief The DoubleGenetic class implements a Genetic Algorithm for
 * global optimization purposes.
 */
class PDOUBLEGENETIC_EXPORT pDoubleGenetic: public Optimizer, pDoubleGeneticInterface
{
private:
    vector<Data> chromosome;
    vector<Data> children;
    Data        fitnessArray;
    int         generation;
    Data lmargin,rmargin;
    //for termination rule

    void	getTournamentElement(int subCluster, Data &x);
    void	tournament(int subCluster, Data &p1,Data &p2);
    int		randomChromosome(int subCluster);
    void    doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2);
    double randMToN(double M, double N);
    int randMToN(int M, int N);

    std::chrono::time_point<std::chrono::system_clock> before, after;
    bool checkSubCluster(int subClusterName);
    int subClusterEnable, similarityMaxCount, double_generations, subCluster, double_chromosomes, population, centers, minK, pNumber, propagateRate,endPos;
    vector<int> similarityCurrentCount;
    vector<double> sum, newSum, MO, newMO, bestF2xInCluster, bestF2xInClusterOLD, bestx,doublebox_xx1 ,doublebox_xx2,doublebox_variance,doublebox_stopat, oldBesty;
    double dmin, selection_rate, mutation_rate, rate;
    KMeans *kmeans;
    vector<Point> allSamples;
    vector<Point> allmeans;
    QString parallelPropagateMethod;
    QString sample_method, debug;
    QRandomGenerator gen1 =  QRandomGenerator();

    void replace(int subClusterIndex, vector<pair<double, Data>> chrom);
    void propagate();
    int subClusterStartPos(int subClusterIndex);
    int subClusterEndPos(int subClusterIndex);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();


public:
    /**
     * @brief DoubleGenetic the constructor of the class.
     * @param p
     */
    pDoubleGenetic(Problem *p);
    /**
     * @brief Solve, perform the steps of the Genetic algorithm.
     */
    virtual         void Solve();
    virtual         ~pDoubleGenetic();
};

extern "C" PDOUBLEGENETIC_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PDOUBLEGENETIC_H

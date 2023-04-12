#ifndef PARALLELGENETIC_H
#define PARALLELGENETIC_H

#include "ParallelGenetic_global.h"
# include <Optimus/problem.h>
# include <Optimus/optimizer.h>


class ParallelGeneticInterface
{
public:
    virtual ~ParallelGeneticInterface()
    {

    }
private:
    virtual void calcFitnessArray(int)=0;
    virtual void select(int)=0;
    virtual void crossover(int)=0;
    virtual void mutate(int)=0;
    virtual void	getTournamentElement(int,Data &x)=0;
    virtual void	tournament(int,Data &p1,Data &p2)=0;
public:

};


/**
 * @brief The ParallelGenetic class implements a basic
 * parallel Genetic algorithm that follows the Island
 * paralell model.
 * Parameters
 * ===============================
 * 1. pargen_count, number of chromosomes for every island
 * 2. pargen_iters, maximum number of iterations
 * 3. pargen_islands, the number of parallel threads
 * 4. pargen_srate,  the selection rate
 * 5. pargen_mrate,  the mutation rate
 * 6. pargen_migratecount,   the number of chromosomes the will migrate
 * 7. pargen_tournamentsize, the size of the tournament.
 * 8. pargen_mutationpercent, the change rate in mutation.
 * 9. pargen_migrategeneration, the number of generations
 *    before the migration tooks place.
 * 10. pargen_termination, termination policy. Available values:
 *    master, majority, one.
 * 11. pargen_migrationmethod, Available values: 1to1, 1toN, NtoN, Nto1
 * 12. pargen_termmethod,  Available values: doublebox, similarity
 * 13. pargen_similaritycount, Number of generations for similarity check
 * 14. pargen_debug, If it is yes the messages will be appear.
 */
class PARALLELGENETIC_EXPORT ParallelGenetic: public
                   Optimizer, ParallelGeneticInterface
{
private:
    //params
    int pargen_count;// number of chromosomes for every island
    int pargen_iters;//maximum number of iterations
    int pargen_islands;//the number of parallel threads
    double pargen_srate;//the selection rate
    double pargen_mrate;//the mutation rate
    double pargen_mutationpercent;
    int pargen_migratecount;
    int pargen_tournamentsize;//the size of the tournament
    int pargen_migrategeneration;
    QString pargen_termination;
    QString pargen_migrationmethod;
    QString pargen_termmethod;
    int     pargen_similaritycount;
    QString pargen_debug;

    vector<Data> chromosome;
    vector<Data> children;
    Data        fitnessArray;
    int         generation;
    Data        lmargin,rmargin;
    int         islands;


    //for termination rule doublebox
    Data xx1,xx2,stopat,variance,oldBesty;
    double xx1_master,xx2_master,stopat_master,variance_master,
       oldBesty_master;

    //for termination rule similarity
    Data similarityValue;
    vector<int> similarityCount;
    double similarityValue_master;
    int similarityCount_master;

    void    calcFitnessArray(int islandIndex);
    void    select(int islandIndex);
    void    crossover(int islandIndex);
    void    mutate(int islandIndex);
    void	getTournamentElement(int islandIndex,Data &x);
    void	tournament(int islandIndex,Data &p1,Data &p2);
    int		randomChromosome(int islandIndex);
    int     subClusterStartPos(int subClusterIndex);
    int     subClusterEndPos(int subClusterIndex);
    void    doubleCrossover(Data &parent1, Data &parent2, Data &children1, Data &children2);
    bool    updateTerminationRule(int islandIndex);
    bool    updateMasterTerminationRule();
    int     getBestIndex();
    void    migrateFromIslandtoIsland(int from,int to);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void    migrate();

public:

    ParallelGenetic(Problem *p);

    virtual         ~ParallelGenetic();
};

extern "C" PARALLELGENETIC_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PARALLELGENETIC_H

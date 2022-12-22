#ifndef PARALLELDE_H
#define PARALLELDE_H

#include "ParallelDe_global.h"
#include <Optimus/optimizer.h>
#include <Optimus/tolmin.h>

class ParDEInterface
{
public:
    virtual ~ParDEInterface()
    {
    }
};

/**
 * @brief The ParallelDe class implements the Parallel Differential evolution method.
 * Available Method Params are:
 * 1) parde_termination: The termination rule used. Available values: maxiters,similarity,doublebox.
 * 2) parde_similarityMax: The maximum allowed number of generations for the similarity stopping rule.
 * 3) parde_agents: The number of agents for every Parallel Island.
 * 4) parde_generations: The maximum number of generations allowed.
 * 5) parde_islands: The number of parallel islands for the method.
 * 6) parde_cr: The crossover probability.
 * 7) parde_weight_method: The differential weight method. The available values are: random, ali, constant.
 * 8) parde_f: The value for the differential weight when parde_weight_method=="constant".
 * 9) parde_propagate_rate: The number of generations before the propagation starts.
 * 10) parde_selection_method: The selection method used to select atoms in crossover. Available values
 *     are: random, tournament.
 * 11) parde_propagate_method: The used propagation method between islands.
 *     Available values are: 1to1,1toN,Nto1,NtoN
 */
class PARALLELDE_EXPORT ParallelDe : public Optimizer, ParDEInterface
{
private:
    Matrix population;

    double parde_F, parde_CR;
    int islands, agents,generation, parde_generations, similarity_max_count, parde_islands_enable,parde_propagate_rate;
    vector<int> bestIslandIndex,similarity_current_count;
    vector<double> similarity_best_value,lmargin, rmargin,bestIslandValues,fitness_array,sum,newSum, MO, newMO;
    vector<double> doublebox_xx1, doublebox_xx2, doublebox_best_value, doublebox_variance, doublebox_stopat;
    QString parde_termination, parde_propagate_method, parde_weight_method;
    std::chrono::time_point<std::chrono::system_clock> before, after;

    int selectAtom(int islandIndex);
    int tournament(int islandIndex, int tsize = 8);
    int islandStartPos(int islandIndex);
    int islandEndPos(int islandIndex);
    void propagateIslandValues();
    void getBestValue(int &index, double &value);
    void getBestValue(int islandName, int &index, double &value);
    double getDifferentialWeight();
    void replaceValueInIsland(int islandIndex, Data &x, double &y);
    bool checkIsland(int islandName);
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();

    //for similarity stopping rule
    double global_sim_value;
    int global_sim_count;

    double start,end;

public:
    ParallelDe(Problem *p);
    virtual ~ParallelDe();
};

extern "C" PARALLELDE_EXPORT Optimizer *createOptimizer(Problem *p);

#endif // PARALLELDE_H

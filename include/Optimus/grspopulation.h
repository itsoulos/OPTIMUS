#ifndef __GRSPOPULATION__H
#include "program.h"
#include <random>

/**
 * @brief The GrsPopulation class implements the Genetic Population used in the GRS local optimizer.
 */
class GrsPopulation
{
private:
    int **children;
    int **genome;
    double *fitness_array;
    double mutation_rate, selection_rate;
    int genome_count;
    int genome_size;
    int generation;
    Program *program;
    /**
     * @brief fitness.
     * @param g is vector of integers
     * @return the best value of fitness.
     */
    double fitness(vector<int> &g);
    /**
     * @brief select,selection process .
     */
    void select();
    /**
     * @brief crossover, crossover process .
     */
    void crossover();
    /**
     * @brief mutate, mutation process .
     */
    void mutate();
    /**
     * @brief calcFitnessArray, calculation fitness Array .
     */
    void calcFitnessArray();
    int elitism;
    /*
     * uniform distribution machine
     */
    std::mt19937 *rng;
    std::uniform_int_distribution<std::mt19937::result_type> *dist;

    int randint();

public:
    /** @brief GrsPopulation the constructor of the class.
     *  @param p is the objective problem to be minimized
     *  @param gcount is the number of chromosomes
     *  @param gsize is the lenght of chromosome
     * **/
    GrsPopulation(int gcount, int gsize, Program *p);
    /**
     * @brief setElitism.
     * @param s the position of best chromosome.
     */
    void setElitism(int s);
    /**
     * @brief getGeneration
     * @return the number of generation.
     */
    int getGeneration() const;
    /**
     * @brief getGount
     * @return the number of genome Count.
     */
    int getCount() const;
    /**
     * @brief getSize
     * @return the number of genome size.
     */
    int getSize() const;
    /**
     * @brief nextGeneration execution of procedures:
     * mutate, calcFitnessArray, select, crossover, ++generation.
     */
    void nextGeneration();
    /**
     * @brief setMutationRate.
     * @param r the mutation rate.
     */
    void setMutationRate(double r);
    /**
     * @brief setSelectionRate.
     * @param r the selection rate.
     */
    void setSelectionRate(double r);
    /**
     * @brief getSelectionRate
     * @return the selection rate.
     */
    double getSelectionRate() const;
    /**
     * @brief getMutationRate
     * @return the mutation rate.
     */
    double getMutationRate() const;
    /**
     * @brief getBestFittnessRate
     * @return the best minimum value.
     */
    double getBestFitness() const;
    /**
     * @brief evaluateBestFitness
     * @return the evaluate best value.
     */
    double evaluateBestFitness();
    /**
     * @brief getBestGenome
     * @return the vector of best genome.
     */
    vector<int> getBestGenome() const;
    /**
     * @brief reset, restore population in random values
     */
    void reset();
    /**
     * @brief ~GrsPopulation is the destructor of class.
     */
    ~GrsPopulation();
};
#define __GRSPOPULATION__H
#endif

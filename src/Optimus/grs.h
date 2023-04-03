#ifndef __GRS__H
#define __GRS__H

#include <rlsprogram.h>
#include <grspopulation.h>
#include <collection.h>

/**
 * @brief The Grs class implements the Genetic Random Search local optimization method
 */
class Grs
{
private:
    int maxGenerations;
    Problem *problem;
    RlsProgram *program;
    GrsPopulation *pop;

public:
    /** @brief Grs, the constructor of the class.
     * The @param p is the objective problem
     * to be minimized **/
    Grs(Problem *p);
    /**
     * @brief getGenomeCount.
     * @return the number of genes.
     */
    int getGenomeCount() const;
    /**
     * @brief getGenomeLength.
     * @return the length of genes.
     */
    int getGenomeLength() const;
    /**
     * @brief getMaxGenerations
     * @return the number of generations.
     */
    int getMaxGenerations() const;
    /**
     * @brief setGenomeCount.
     * @param c the number of genes.
     */
    void setGenomeCount(int c);
    /**
     * @brief setGenomeLength.
     * @param l the length of genes.
     */
    void setGenomeLength(int l);
    /**
     * @brief setMaxGenerations(.
     * @param g the number of generations.
     */
    void setMaxGenerations(int g);
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
     * @brief setSelectionRate.
     * @param s the selection rate.
     */
    void setSelectionRate(double s);
    /**
     * @brief setMutationRate.
     * @param m the mutation rate.
     */
    void setMutationRate(double m);
    /**
     * @brief getFunctionEvaluations
     * @return the number of evaluations.
     */
    int getFunctionEvaluations();
    /**
     * @brief getGradientEvaluations
     * @return the number of gradient evaluations.
     */
    int getGradientEvaluations();
    /**
     * @brief solve is the solver of grs.
     * @param x the starting point.
     * @param y f(x) the minimimun value.
     */
    void Solve(Data &x, double &y);
    /**
     * @brief getMinimum
     * @return the nminimum value.
     */
    double getMinimum() const;
    /**
     * @brief ~Grs is the destructor of class.
     */
    ~Grs();
};
#endif

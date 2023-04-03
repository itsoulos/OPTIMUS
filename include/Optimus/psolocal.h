#ifndef PSOLOCAL_H
#define PSOLOCAL_H
#include <problem.h>

/**
 * @brief The PsoLocal class executes the PSO optimizer as local optimization method
 * without derivatives.
 */
class PsoLocal
{
private:
    Problem *myProblem;
    vector<Data> position;
    vector<Data> bestPosition;
    vector<Data> velocity;
    Data bestg;
    Data fitnessArray;
    Data bestFitnessArray;
    double r1, r2;
    double percentageTheta;
    double bestFitness;
    Data lmargin;
    Data rmargin;

public:
    /** @brief PsoLocal, the constructor of the class.
     *  @param p is the objective problem to be minimized
     *  @param particles, the number of particles.
     **/
    PsoLocal(Problem *p, int particles);
    /** @brief PsoLocal, the constructor of the class.
     *  @param p is the objective problem to be minimized
     *  @param initialPoint is the starting point
     *  @param particles, the number of particles.
     *  @param theta, speed multiplier.
     **/
    PsoLocal(Problem *p, Data &initialPoint, int particles, double theta);
    /** @brief setR1.
     *  @param r, factor r1
     **/
    void setR1(double r);
    /** @brief setR2.
     *  @param r, factor r2
     **/
    void setR2(double r);
    /** @brief Solve, local solver particle swamp optimization.
     *  @param generations, is number of generations
     **/
    void Solve(int generations);
    /**
     * @brief gestBestParticle
     * @return return the particle with best position.
     */
    Data gestBestParticle() const;
    /**
     * @brief ~nPsoLocal is the destructor of class.
     */
    ~PsoLocal();
};

#endif // PSOLOCAL_H

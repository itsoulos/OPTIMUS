#ifndef NELDERMEAD_H
#define NELDERMEAD_H

#include "nelderMead_global.h"
#include "problem.h"

/**
 * @brief The nelderMead class implements the Nelder Mead local optimizer.
 */
class NELDERMEAD_EXPORT nelderMead
{
private:
    Problem *myProblem;
    /** @brief vector of couples firs t=> f(sample), second => sample.
     **/
    vector<pair<double, Data>> population;
    int max_generations;
    Data lmargin, rmargin, fitness_array, bestPoint, centerPoint, reflectedPoint,
        expandedPoint, contractedPoint;
    double x1, x2, stopat, variance, ybestPoint, sum, newSum, alpha, gamma, ro, sigma,
        worst, secondWorst, yreflectedPoint, yexpandedPoint, ycontractedPoint;
    int generation, population_count, population_size, bestMinIndex, n;
    /** @brief order, sorting progress.
     **/
    void order();
    /** @brief center, center finding.
     **/
    void center();
    /** @brief reflection, reflection progress.
     **/
    void reflection();
    /** @brief expansion, expansion progress.
     **/
    void expansion();
    /** @brief contraction, contraction internal progress.
     **/
    void contraction();
    /** @brief contractionB, contraction external progress.
     **/
    void contractionB();
    /** @brief shrink, shrinking progress.
     **/
    void shrink();

public:
    /** @brief nelderMead, the constructor of the class.
     *  @param p is the objective problem to be minimized
     *  @param x is the starting point
     *  @param y f(x), minimum.
     * **/
    nelderMead(Problem *p, Data &x, double y);
    /**
     * @brief terminated is the termination rules
     * @return true when the algorithm should terminate.
     */
    bool terminated();
    /** @brief step, execution of the algorithm at each step.
     **/
    void step();
    /** @brief init, initialization of the algorithm.
     **/
    void init();
    /** @brief done, finalize the algorithm.
     **/
    void done();
    /**
     * @brief getBestX.
     * @return the best sample.
     */
    Data getBestX() const;
    /**
     * @brief getBestX.
     * @return f(x) from best sample.
     */
    double getBestY() const;
    /**
     * @brief ~nelderMead is the destructor of class.
     */
    ~nelderMead();
};

#endif // NELDERMEAD_H

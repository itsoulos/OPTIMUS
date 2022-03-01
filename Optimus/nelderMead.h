#ifndef NELDERMEAD_H
#define NELDERMEAD_H

#include "nelderMead_global.h"
#include "problem.h"

class NELDERMEAD_EXPORT nelderMead
{
private:
    Problem *myProblem;

    vector<pair<double, Data>> population;
    int max_generations;
    Data lmargin, rmargin, fitness_array, bestPoint, centerPoint, reflectedPoint,
            expandedPoint, contractedPoint;
    double x1, x2, stopat, variance, ybestPoint, sum, newSum, alpha, gamma, ro, sigma,
    worst, secondWorst, yreflectedPoint, yexpandedPoint, ycontractedPoint;
    int generation, population_count, population_size, bestMinIndex, n;
    void order();
    void center();
    void reflection();
    void expansion();
    void contraction();
    void contractionB();
    void shrink();

public:
    nelderMead(Problem *p,Data &x,double y);
    bool terminated();
    void step();
    void init();
    void done();
    Data getBestX() const;
    double getBestY() const;
    ~nelderMead();
};


#endif // NELDERMEAD_H

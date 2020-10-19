#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H
# include "problem.h"

class GradientDescent
{
private:
    Problem *myProblem;
    int     iters;
    double  rate;
public:
    GradientDescent(Problem *p);
    double  Solve(Data &x);
    void    setRate(double r);
    double  getRate() const;
    void    setIters(int i);
    int     getIters() const;
};

#endif // GRADIENTDESCENT_H

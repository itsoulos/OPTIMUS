#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H
# include "problem.h"
/** @brief Implement the steepest descent optimization method **/
class GradientDescent
{
private:
    Problem *myProblem;
    int     iters;
    double  rate;
public:

    /** The constructor of the class.
     * The @param p is the objective problem
     * to be minimized **/
    GradientDescent(Problem *p);

    /** @param x is the starting point of the
     *  optimization procedure
     *  @return the estimated minimum **/
    double  Solve(Data &x);

    /** @param r is  the required learning rate for the steepest
     *  descent method. r should be between 0 and 1.**/
    void    setRate(double r);

    /** @return the learning rate of the steepest descent **/
    double  getRate() const;

    /** @param i is the required maximum number of
     *  iterations allowed for the steepest descent method **/
    void    setIters(int i);

    /** @return the number of maximum iteratiosn allowed **/
    int     getIters() const;
};

#endif // GRADIENTDESCENT_H

#ifndef ADAM_H
#define ADAM_H
# include "problem.h"

/** @brief The Adam local optimization method
 * described in https://machinelearningmastery.com/adam-optimization-algorithm-for-deep-learning/
 */
class Adam
{
private:
    Problem *myProblem;
    double learningRate;
    int     iterations;
    Data M_0,U_0,rM,lM;
    double b1,b2;
public:
    /** Constructor of the Method
     * @param p is the problem to be optimized**/
    Adam(Problem *p);

    /** @param x is the starting point for the optimizer.
     *  @return the estimated minimum*/
    double  Solve(Data &x);

    /** @param b, the value for the b1  parameter of the
     *  Adam algorithm. b should be between 0 and 1.*/
    void    setB1(double b);

    /** @param b, the value for the b2 parameter of the
     *  Adam algorithm. b should be between 0 and 1.*/
    void    setB2(double b);

    /** @return the value of the b1 parameter of
     * the Adam optimizer**/
    double  getB1() const;

    /** @return the value of the b2 parameter of
     * the Adam optimizer**/
    double  getB2() const;

    /** @param r is the learning rate for the
     *  Adam method. r should be between 0 and 1**/
    void    setLearningRate(double r);

    /** @return the learning rate of the Adam**/
    double  getLearningRate() const;

    /** @param it is the maximum number of iterations
     *  for the Adam optimizer. it should be >0 **/
    void    setIterations(int it);

    /** @return the value of maximum number of iterations allowed for adam **/
    int     getIterations() const;
};

#endif // ADAM_H

#ifndef PROBLEM_H
#define PROBLEM_H
#include "intervalproblem.h"
#define integer long
#define double double
#define real float
typedef long int ftnlen;
typedef long int logical;

/**
 * @brief The Problem class implements the objective function to be optimized.
 */
class Problem
{
private:
    IntervalProblem *problem;
    int dimension;
    Data lmargin, rmargin;
    int functionCalls;
    Data bestx, tempx, tempg;
    double besty;

public:
    /** @brief Problem the constructor of the class.
     * 	@param p is the objective problem to be minimized
     * **/
    Problem(IntervalProblem *p);
    /** @brief Problem the constructor of the class.
     * 	@param p is the objective problem to be minimized
     *  @param m is the data of objective problem
     * **/
    Problem(IntervalProblem *p, IntervalData &m);
    /**
     * @brief getDimension
     * @return the dimension of objective problem.
     */
    int getDimension() const;
    /**
     * @brief getLeftMargin
     * @return the left margin.
     */
    Data getLeftMargin() const;
    /**
     * @brief getRightMargin
     * @return the right margin.
     */
    Data getRightMargin() const;
    /**
     * @brief getLeftMarginx
     * @return the left marginx.
     */
    void getleftmarginx(double *);
    /**
     * @brief getRightMarginx
     * @return the right marginx.
     */
    void getrightmarginx(double *);
    /**
     * @brief funmin
     * @param x is the sample
     * @return f(sample), minimum value.
     */
    double funmin(Data &x);
    /**
     * @brief funmin
     * @param x is the sample
     * @return f(sample), minimum value.
     */
    double funmin(double *x);
    /**
     * @brief granal
     * @param x is the sample
     * @param g if granal of sample.
     */
    void granal(Data &x, Data &g);
    /**
     * @brief granal
     * @param x is the sample
     * @param g if granal of sample.
     */
    void granal(double *x, double *g);
    /**
     * @brief getRandomPoint
     * @return one random point (vector of double).
     */
    Data getRandomPoint();
    /**
     * @brief getUniformRandomPoint
     * @return one uniform random point (vector of double).
     */
    Data getUniformRandomPoint();
    /**
     * @brief done
     * @return json object (finalize the problem).
     */
    QJsonObject done(Data &x);
    /**
     * @brief getGradientCriterion
     * @param x1 first sample.
     * @param x2 second sample.
     * @return true when the params x1, x2 is to closed.
     */
    bool getGradientCriterion(Data &x1, Data &x2);
    /**
     * @brief randomDouble
     * @return random double (0-1).
     */
    double randomDouble();
    /**
     * @brief isPointIn
     * @param x, one sample
     * @return true when param x is between margins.
     */
    bool isPointIn(Data &x);
    /**
     * @brief getFunctionCalls
     * @return the number of fuction calls (speed criterion) .
     */
    int getFunctionCalls();
    /**
     * @brief resetFunctionCalls, nullification of function calls
     */
    void resetFunctionCalls();
    /**
     * @brief resetFunctionCalls, nullification of function calls
     * @param settings, setiings of problem.
     */
    void resetFunctionCalls(QJsonObject &settings);
    /**
     * @brief getBestx
     * @return the best sample .
     */
    Data getBestx();
    /**
     * @brief getBesty
     * @return value f(sample), minimum .
     */
    double getBesty();
    /**
     * @brief getVolume
     * @return value.
     */
    double getVolume();
};

extern double getDistance(Data &x1, Data &x2);

#endif // PROBLEM_H

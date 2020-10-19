#ifndef ADAM_H
#define ADAM_H
# include "problem.h"

class Adam
{
private:
    Problem *myProblem;
    double learningRate;
    int     iterations;
    Data M_0,U_0,rM,lM;
    double b1,b2;
public:
    Adam(Problem *p);
    double  Solve(Data &x);
    void    setB1(double b);
    void    setB2(double b);
    double  getB1() const;
    double  getB2() const;
    void    setLearningRate(double r);
    double  getLearningRate() const;
    void    setIterations(int it);
    int     getIterations() const;
};

#endif // ADAM_H

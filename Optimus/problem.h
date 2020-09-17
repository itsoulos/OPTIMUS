#ifndef PROBLEM_H
#define PROBLEM_H
# include "intervalproblem.h"

class Problem
{
private:
    IntervalProblem *problem;
    int dimension;
    Data lmargin,rmargin;
    int functionCalls;
    Data bestx;
    double besty;
public:
    Problem(IntervalProblem *p);
    Problem(IntervalProblem *p,IntervalData &m);
    int         getDimension() const;
    Data        getLeftMargin() const;
    Data        getRightMargin() const;
    double      funmin(Data &x);
    void        granal(Data &x,Data &g);
    Data        getRandomPoint();
    Data        getUniformRandomPoint();
    QJsonObject done(Data &x);
bool     getGradientCriterion(Data &x1,Data &x2);
    double      randomDouble();
    bool        isPointIn(Data &x);
    int		getFunctionCalls();
    void	resetFunctionCalls();
    Data    getBestx();
    double  getBesty();
};

extern double getDistance(Data &x1,Data &x2);

#endif // PROBLEM_H

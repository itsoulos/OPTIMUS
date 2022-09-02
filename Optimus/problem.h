#ifndef PROBLEM_H
#define PROBLEM_H
# include "intervalproblem.h"
# define integer long
# define double double
# define real float
typedef long int ftnlen;
typedef long int logical;
class Problem
{
private:
    IntervalProblem *problem;
    int dimension;
    Data lmargin,rmargin;
    int functionCalls;
    Data bestx,tempx,tempg;
    double besty;
public:
    Problem(IntervalProblem *p);
    Problem(IntervalProblem *p,IntervalData &m);
    int         getDimension() const;
    Data        getLeftMargin() const;
    Data        getRightMargin() const;
    void        getleftmarginx(double *);
    void        getrightmarginx(double *);
    double      funmin(Data &x);
    double      funmin(double *x);
    void        granal(Data &x,Data &g);
    void        granal(double *x,double *g);
    Data        getRandomPoint();
    Data        getUniformRandomPoint();
    QJsonObject done(Data &x);
bool     getGradientCriterion(Data &x1,Data &x2);
    double      randomDouble();
    bool        isPointIn(Data &x);
    int		getFunctionCalls();
    void	resetFunctionCalls();
    void    resetFunctionCalls(QJsonObject &settings);
    Data    getBestx();
    double  getBesty();
};

extern double getDistance(Data &x1,Data &x2);

#endif // PROBLEM_H

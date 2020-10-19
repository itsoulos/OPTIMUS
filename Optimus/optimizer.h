#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include "problem.h"
# include "tolmin.h"
# include "gradientdescent.h"
# include "adam.h"
class Optimizer
{
protected:
    Problem     *myProblem;
    QJsonObject params;
    int     threads;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
public:
    Optimizer(Problem *p);
    void            setProblem(Problem *p);
    virtual void    setSettings(QJsonObject settings);
    void            setThreads(int t);
    double          localSearch(Data &x);
    Problem         *getProblem();
    virtual         void Solve();
    void            addParameter(QString name,QString value,QString description);
    QString         getParameter(QString name);
    QString         getParameterDescription(QString name);
    QJsonObject     getParameters();
};

#endif // OPTIMIZER_H

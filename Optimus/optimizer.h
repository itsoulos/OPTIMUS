#ifndef OPTIMIZER_H
#define OPTIMIZER_H
#include "problem.h"
# include "tolmin.h"
# include "gradientdescent.h"
# include "adam.h"
# include "lbfgs.h"
#include "nelderMead.h"
# include "uniformsampler.h"
# include "rbfsampler.h"
# include "neuralsampler.h"
# include "maxwellsampler.h"
# include "triangularsampler.h"
class Optimizer
{
protected:
    Problem     *myProblem;
    QJsonObject params;
    ProblemSampler *defaultSampler;
    int     threads;
    virtual bool terminated();
    virtual void step();
    virtual void init();
    virtual void done();
    void    prepareSampler();
public:
    Optimizer(Problem *p);
    void        sampleFromProblem(int &N,Matrix &xsample,Data &ysample);
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
    ~Optimizer();
};

#endif // OPTIMIZER_H

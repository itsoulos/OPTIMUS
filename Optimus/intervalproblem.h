#ifndef INTERVALPROBLEM_H
#define INTERVALPROBLEM_H
# include <vector>
# include <iostream>
# include <QVector>
# include <QJsonObject>
# include "interval.h"
# include <QRandomGenerator>
using namespace  std;
typedef vector<double> Data;
typedef vector<Interval> IntervalData;

/** This class used to represent the objective problem
 * as an interval problem. Objects from this class will
   be used as fields in Problem class.**/

class IntervalProblem
{
protected:
    int                 dimension;
    IntervalData        margin;
    const int     maxSearchDepth=5;
    QRandomGenerator randGen;
    int threads;
    QVector<QRandomGenerator> threadGen;
public:
    /** @param d is the dimension of the objective problem **/
    IntervalProblem(int d);

    /** @param d is the dimension of the problem. The method
     *  modifies the dimension of the class **/
    void                setDimension(int d);


    void                setMargins(IntervalData &m);
    IntervalData        getMargins() const;
    int                 getDimension() const;
    bool                isPointIn(Data &x) const;
    bool                isPointIn(IntervalData &x) const;
    IntervalData        createRandomInterval();
    void                randomize(IntervalData &x,int pos);
    virtual             void init(QJsonObject x);
    virtual             void setParameter(QString name,QVariant value);
    virtual double      funmin(Data &x);
    virtual void        granal(Data &x,Data &g);
    virtual QJsonObject done(Data &x);
    bool                lowerValue(Interval &a,Interval &b);
    double              getVolume() const;
    double              getVolume(IntervalData &x) const;
    void                boundInterval(IntervalData &x);
    void                printData(IntervalData &x);
    virtual void                getSample(Data &x);
    double              randomDouble();
    void                setRandomSeed(int k);
    void                setRandomSeedForThreads(int k,int t);
    virtual             ~IntervalProblem();
};

#endif // INTERVALPROBLEM_H

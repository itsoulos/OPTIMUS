#ifndef DLLPROBLEM_H
#define DLLPROBLEM_H
#include "intervalproblem.h"
# include <QLibrary>
# include <QVariant>
//
typedef int (*GETDIMENSION)();
typedef void(*GETMARGINS)(vector<Interval>&);
typedef double(*FUNMIN)(Data &x);
typedef void(*GRANAL)(Data &x,Data &g);
typedef QJsonObject(*DONE)(Data &x);
typedef void(*INIT)(QJsonObject x);
typedef void(*SETP)(QString,QVariant);
typedef void(*SAMPLER)(Data &x);

class DllProblem :public IntervalProblem
{
private:
      QLibrary      *library;
      GETDIMENSION  getdim;
      GETMARGINS    GetMargins;
      FUNMIN        Funmin;
      GRANAL        Granal;
      DONE          Done;
      INIT          Init;
      SETP          Setp;
      SAMPLER       Sampler;
public:
    DllProblem(QString filename,QJsonObject settings);
    virtual void init(QJsonObject x);
    virtual double funmin(Data &x);
    virtual void granal(Data &x,Data &g);
    virtual QJsonObject done(Data &x);
    virtual void  getSample(Data &x);
    virtual void setParameter(QString name, QVariant value);
    virtual ~DllProblem();
};

#endif // DLLPROBLEM_H

#ifndef DLLPROBLEM_H
#define DLLPROBLEM_H
#include "intervalproblem.h"
# include <QLibrary>
# include <QVariant>
typedef int (*GETDIMENSION)();
typedef void(*GETMARGINS)(vector<Interval>&);
typedef double(*FUNMIN)(Data &x);
typedef void(*GRANAL)(Data &x,Data &g);
typedef QJsonObject(*DONE)(Data &x);
typedef void(*INIT)(QJsonObject x);
typedef void(*SETP)(QString,QVariant);
typedef void(*SAMPLER)(Data &x);



/** @brief Abstract class for loading
 * global optimization problems from shared libraries **/
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
      /** The constructor of the class
       * @param filename is the dll file
       * @param settings is the initial settings of the problem **/
    DllProblem(QString filename,QJsonObject settings);

    /** Initialize the objective problem using the @param x**/
    virtual void init(QJsonObject x);

    /** @return the value of the objective function at point of the @param x**/
    virtual double funmin(Data &x);

    /** Estimate all the gradient for every index in @param x
     * and @return the result in the @param g**/
    virtual void granal(Data &x,Data &g);

    /** Perform the done procedure on @param x.
     * @return a json object with the result of the done procedure**/
    virtual QJsonObject done(Data &x);

    /** @return on @param x a new sample from the problem*/
    virtual void  getSample(Data &x);

    /** Abstract method to change a parameter of the problem
     * @param name is the name of the parameter
     * @value is the actual value of the parameter **/
    virtual void setParameter(QString name, QVariant value);

    /** The destructor of the class**/
    virtual ~DllProblem();
};

#endif // DLLPROBLEM_H

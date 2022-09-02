#ifndef RULERMODEL_H
#define RULERMODEL_H

#include "demodel.h"
# include <rulercprogram.h>
# include <deprogram.h>
# include <QStringList>
class RulerModel : public DeModel, DeProgram
{
private:
    int dimension;
    vector<FunctionParser> mparser;
    RulerCprogram *cprogram;
    string vars;
    string lastExpr;
    QStringList list;
    vector<string> clist;
public:
    RulerModel(int d);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    void    setChromosome(vector<int> &g);
    string  printF(vector<int> &genome);
    virtual ~RulerModel();
};

#endif // RULERMODEL_H

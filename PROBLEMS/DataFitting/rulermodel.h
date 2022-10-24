#ifndef RULERMODEL_H
#define RULERMODEL_H

#include "datamodel.h"
# include <rulercprogram.h>
# include <deprogram.h>
# include <QStringList>
class RulerModel : public DataModel, DeProgram
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
    RulerModel(DataSet *tr,DataSet *tt);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    void    setChromosome(vector<int> &g);
    string  printF(vector<int> &genome);
    virtual ~RulerModel();
};

#endif // RULERMODEL_H

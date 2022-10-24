#ifndef MLPMODEL_H
#define MLPMODEL_H

#include "datamodel.h"

class MlpModel : public DataModel
{
private:
    Data weight;
    int  dimension;//one for odes and two for pdes
    double sig(double x);
    double sigder(double x);
    double sigder2(double x);
public:
    MlpModel(int nweights,DataSet *tr,DataSet *tt);
    int     getNWeights() const;
    Data    getWeights();
    void    setWeights(Data &w);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    virtual ~MlpModel();
};

#endif // MLPMODEL_H

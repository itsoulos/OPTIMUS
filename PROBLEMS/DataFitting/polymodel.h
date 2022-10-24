#ifndef POLYMODEL_H
#define POLYMODEL_H

#include "datamodel.h"

class PolyModel : public DataModel
{
private:
    int dimension;
    Data weights;
public:
    PolyModel(int m,DataSet *tr,DataSet *tt);
    void    setWeights(Data &x);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    virtual ~PolyModel();
};

#endif // POLYMODEL_H

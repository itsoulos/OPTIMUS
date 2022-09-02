#ifndef POLYMODEL_H
#define POLYMODEL_H

#include "demodel.h"

class PolyModel : public DeModel
{
private:
    int dimension;
    Data weights;
public:
    PolyModel(int d,int m);
    void    setWeights(Data &x);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    virtual ~PolyModel();
};

#endif // POLYMODEL_H

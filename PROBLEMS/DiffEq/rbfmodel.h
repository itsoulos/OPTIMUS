#ifndef RBFMODEL_H
#define RBFMODEL_H

#include "demodel.h"
# include <deprogram.h>

class RbfModel : public DeModel
{
private:
    Data weight;
    int  dimension;
    vector<Data>    center;
    Data            variance;
    double      getDistance(Data &x,Data &y);
    double      gaussian(Data &x,Data &m,double v);
    double      gaussianDerivative(Data &x,Data &m,double v,int pos);
    double      gaussianSecondDerivative(Data &x,Data &m,double v,int pos);
public:
    RbfModel(int d,int nweights);
    int     getNWeights() const;
    Data    getWeights();
    void    setWeights(Data &w);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    void    setVariables(Data &w);
    int     getVariablesSize() const;
    virtual ~RbfModel();
};

#endif // RBFMODEL_H

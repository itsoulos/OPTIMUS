#ifndef GDFMODEL_H
#define GDFMODEL_H

#include "demodel.h"
# include <cprogram.h>

class GdfModel : public DeModel
{
private:
    int dimension;
    Cprogram *program;
public:
    GdfModel(int d);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    virtual ~GdfModel();
};

#endif // GDFMODEL_H

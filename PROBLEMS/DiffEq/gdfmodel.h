#ifndef GDFMODEL_H
#define GDFMODEL_H

#include "demodel.h"
# include <cprogram.h>
# include <deprogram.h>

class GdfModel : public DeModel
{
private:
    int dimension;
    Cprogram *cprogram;
    DeProgram  *program;
    bool lastError = false;
    string  lastExpr = "";
public:
    GdfModel(int d);
    void    setChromosome(vector<int> &g);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    string  getLastExpr() const;
    virtual ~GdfModel();
};

#endif // GDFMODEL_H

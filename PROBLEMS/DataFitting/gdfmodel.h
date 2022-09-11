#ifndef GDFMODEL_H
#define GDFMODEL_H

#include "datamodel.h"
# include <string>
# include <cprogram.h>
# include <deprogram.h>
using namespace std;

class GdfModel : public DataModel
{
private:
    int dimension;
    Cprogram *cprogram;
    DeProgram  *program;
    bool lastError = false;
    string  lastExpr = "";
public:
    GdfModel(Dataset *tr,Dataset *tt);
    void    setChromosome(vector<int> &g);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    string  getLastExpr() const;
    virtual ~GdfModel();
};

#endif // GDFMODEL_H

#ifndef NNCMODEL_H
#define NNCMODEL_H

#include "datamodel.h"
# include <neuralprogram.h>
class NncModel : public DataModel
{
private:
    NeuralProgram *program;
    int dimension;
    bool lastError = false;
public:
    NncModel(Dataset *tr,Dataset *tt);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    void    setChromosome(vector<int> &g);
    virtual ~NncModel();
};

#endif // NNCMODEL_H

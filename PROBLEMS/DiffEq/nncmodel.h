#ifndef NNCMODEL_H
#define NNCMODEL_H

#include "demodel.h"
# include <neuralprogram.h>
class NncModel : public DeModel
{
private:
    NeuralProgram *program;
    int dimension;
    bool lastError = false;
public:
    NncModel(int d);
    virtual double eval(Data &x);
    virtual double evalDeriv(Data &x,int pos);
    virtual double evalSecondDeriv(Data &x,int pos);
    void    setChromosome(vector<int> &g);
    virtual ~NncModel();
};

#endif // NNCMODEL_H

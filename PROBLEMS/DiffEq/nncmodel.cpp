#include "nncmodel.h"

NncModel::NncModel(int d)
{
    dimension = d;
    program = new NeuralProgram();
    program->setDimension(dimension);
}

double  NncModel::eval(Data &x)
{
    return program->neuralparser->eval(x);
}

double  NncModel::evalDeriv(Data &x,int pos)
{
    return program->neuralparser->evalDeriv(x,pos);
}

double NncModel::evalSecondDeriv(Data &x,int pos)
{
    return program->neuralparser->evalDeriv2(x,pos);
}

void    NncModel::setChromosome(vector<int> &g)
{
    program->fitness(g);
}

NncModel::~NncModel()
{
    delete program;
}

#include "demodel.h"

DeModel::DeModel()
{

}

void   DeModel::getModelDeriv(Data &x,Data &g)
{
    //to be overloaded

}

void	DeModel::getXDeriv(vector<double> xpoint,int pos,vector<double> &g)
{
    //to be overloaded

}
DeModel::~DeModel()
{

}

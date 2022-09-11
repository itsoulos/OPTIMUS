#include "datamodel.h"

DataModel::DataModel(Dataset *tr,Dataset *tt)
{
    trainSet = tr;
    testSet = tt;
}

double  DataModel::getTrainError()
{
    double sum  = 0.0;
    for(int i=0;i<trainSet->getpatterns();i++)
    {
        Data xp = trainSet->getpoint(i);
        double val = eval(xp);
        double v = trainSet->gety(i);
        sum+= (val -v)*(val -v);
    }
    return sum;
}

double  DataModel::getTestError()
{
    double sum  = 0.0;
    for(int i=0;i<testSet->getpatterns();i++)
    {
        Data xp = testSet->getpoint(i);
        double val = eval(xp);
        double v = testSet->gety(i);
        sum+= (val -v)*(val -v);
    }
    return sum;
}


DataModel::~DataModel()
{

}

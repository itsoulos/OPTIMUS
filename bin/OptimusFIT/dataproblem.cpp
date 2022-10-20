#include "dataproblem.h"

DataProblem::DataProblem(QString trname,QString tsname)
    :IntervalProblem(1)
{
    trainName = trname;
    testName = tsname;
    trainSet = new Dataset(trname);
    testSet = new Dataset(testName);
    setDimension(trainSet->getdimension());
    Data xleft,xright;
    trainSet->getXMargins(xleft,xright);
    IntervalData data;
    data.resize(dimension);
    for(int i=0;i<dimension;i++)
    {
        data[i]=Interval(xleft[i],xright[i]);
    }
    setMargins(data);
}

QString DataProblem::getTrainName() const
{
    return trainName;
}

QString DataProblem::getTestName() const
{
    return testName;
}

int DataProblem::getDimension() const
{
    return dimension;
}

Dataset *DataProblem::getTrainSet()
{
    return trainSet;
}

Dataset *DataProblem::getTestSet()
{
    return testSet;
}

DataProblem::~DataProblem()
{
    if(trainSet!=NULL)
        delete trainSet;
    if(testSet!=NULL)
        delete testSet;
}

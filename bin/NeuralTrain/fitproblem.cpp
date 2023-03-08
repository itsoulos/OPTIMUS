#include "fitproblem.h"

FitProblem::FitProblem(QJsonObject &px)
    :IntervalProblem(1)
{
    parameters = px;
    QString trainName = parameters["trainName"].toString();
    QString testName = parameters["testName"].toString();
    dataProblem = new DataProblem(trainName,testName);
    dataProblemWrapper = new Problem(dataProblem);
    int weights = parameters["weights"].toString().toInt();
    Dataset *train = dataProblem->getTrainSet();
    int patternDimension = train->getdimension();
    int k = (patternDimension + 2)*weights;
    setDimension(k);
    IntervalData data;
    data.resize(k);
    for(int i=0;i<k;i++)
    {
        data[i]=Interval(-100,100);
    }
    setMargins(data);
    network = new NeuralNetwork(train,weights);

}

/*
void    FitProblem::getSample(Data &x)
{
    double left =-0.01;
    double right = 0.01;
    for(int i=0;i<x.size();i++)
    {
        x[i]=left+(right-left)*rand()*1.0/RAND_MAX;
    }
}*/

void        FitProblem::setPhase(int phase,Data &x)
{
    if(phase == BOUNDWEIGHT_PHASE)
        network->disableNormalTrain();
    else
    {
        int weights = parameters["weights"].toString().toInt();
        Dataset *train = dataProblem->getTrainSet();
        int patternDimension = train->getdimension();
        int k = (patternDimension + 2)*weights;
        IntervalData data;
        data.resize(k);
        for(int i=0;i<k;i++)
        {
		double left=0.9 *x[i];
		double right=1.1*x[i];
            data[i]=Interval(left,right);
        }
        setMargins(data);
        network->enableNormalTrain();
    }
}

double      FitProblem::funmin(Data &x)
{
    return network->funmin(x);
}

double      FitProblem::getTrainError()
{
    return network->getTrainError();
}

double      FitProblem::getTestError()
{
    Dataset *test = dataProblem->getTestSet();
    return network->getTestError(test);
}

double      FitProblem::getClassTestError()
{

    Dataset *test = dataProblem->getTestSet();
    return network->getClassTestError(test);
}

QJsonObject FitProblem::done(Data &x)
{
    funmin(x);
    double d1=getTrainError();
    double d2=getTestError();
    double d3=getClassTestError();
    QJsonObject tx;
    tx["trainError"]=d1;
    tx["testError"]=d2;
    tx["classError"]=d3;
    return tx;
}

void        FitProblem::granal(Data &x,Data &g)
{
    network->granal(x,g);
}

FitProblem::~FitProblem()
{

    if(dataProblem!=NULL)
    {
        delete dataProblemWrapper;
        delete dataProblem;
    }
    delete network;
}

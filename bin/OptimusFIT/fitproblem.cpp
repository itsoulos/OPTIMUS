#include "fitproblem.h"

FitProblem::FitProblem(QJsonObject &px)
    :IntervalProblem(1)
{
    parameters = px;
    QString trainName = parameters["trainName"].toString();
    QString testName = parameters["testName"].toString();
    dataProblem = new DataProblem(trainName,testName);
    dataProblemWrapper = new Problem(dataProblem);
    QString model = parameters["model"].toString();
    int weights = parameters["weights"].toString().toInt();
    int nncsize = parameters["nncsize"].toString().toInt();
    if(model == "rbf")
    {
        sampler = new RbfSampler(dataProblemWrapper,weights);
        int k = ((RbfSampler*)sampler)->getParameterSize();
        setDimension(k);
        IntervalData data;
        data.resize(k);
        for(int i=0;i<k;i++)
        {
            data[i]=Interval(0.1,100);
        }
        setMargins(data);
    }
    else
    if(model == "mlp")
    {
        sampler = new NeuralSampler(dataProblemWrapper,weights);
        int k = (dataProblem->getDimension()+2)*weights;
        setDimension(k);
        IntervalData data;
        data.resize(k);
        for(int i=0;i<k;i++)
        {
            data[i]=Interval(-100,100);
        }
        setMargins(data);
    }
    else
    if(model == "nnc")
    {
         sampler = new NNCSampler(dataProblemWrapper);
         setDimension(nncsize);
         IntervalData data;
         data.resize(nncsize);
         for(int i=0;i<nncsize;i++)
         {
             data[i]=Interval(0,255);
         }
         setMargins(data);
    }

    //add samples from train set
    Dataset *train = dataProblem->getTrainSet();
    int patterns = train->getpatterns();
    for(int i=0;i<patterns;i++)
    {
        Data x = train->getx(i);
        double y = train->gety(i);
        sampler->addSampleFromProblem(x,y);

    }
}

double      FitProblem::funmin(Data &x)
{
    QString model = parameters["model"].toString();
    if(model == "rbf")
    {
        ((RbfSampler *)sampler)->setParameters(x);
    }
    else
    if(model == "mlp")
    {
       ((NeuralSampler *)sampler)->setWeights(x);
    }
    else
    if(model == "nnc")
    {
        ((NNCSampler *)sampler)->setParameters(x);
    }
    return getTrainError();
}

double      FitProblem::getTrainError()
{
    double trainError = 0.0;
    Dataset *train = dataProblem->getTrainSet();
    int patterns = train->getpatterns();
    for(int i=0;i<patterns;i++)
    {
        Data xpoint = train->getx(i);
        double ypoint = train->gety(i);
        double v= sampler->eval(xpoint);
        trainError+=(v-ypoint)*(v-ypoint);

    }
    return trainError ;
}

double      FitProblem::getTestError()
{
    double testError = 0.0;
    Dataset *test = dataProblem->getTestSet();
    int patterns = test->getpatterns();
    for(int i=0;i<patterns;i++)
    {
        Data xpoint = test->getx(i);
        double ypoint = test->gety(i);
        double v= sampler->eval(xpoint);
        testError+=(v-ypoint)*(v-ypoint);

    }
    return testError ;
}

double      FitProblem::getClassTestError()
{
    double testError = 0.0;
    Dataset *test = dataProblem->getTestSet();
    Dataset *train = dataProblem->getTestSet();
    int patterns = test->getpatterns();
    Data dclass;
    for(int i=0;i<train->getpatterns();i++)
    {
        bool found =false;
        for(int j=0;j<dclass.size();j++)
            if(fabs(dclass[j]-train->gety(i))<1e-6)
            {
                found=true;
                break;
            }
        if(!found)
            dclass.push_back(train->gety(i));
    }
    for(int i=0;i<patterns;i++)
    {
        Data x = test->getx(i);
        double v = sampler->eval(x);
        int imin=-1;
        double dmin=1e+100;
        for(int j=0;j<dclass.size();j++)
         {
            if(fabs(dclass[j]-v)<dmin)
            {
                dmin = fabs(dclass[j]-v);
                imin = j;
            }
        }
        if(fabs(dclass[dmin]-test->gety(i))>1e-6)
            testError+=1.0;
    }
    return testError*100.0/patterns;
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
    QString model = parameters["model"].toString();
    if(model == "rbf")
    {
        ((RbfSampler *)sampler)->setParameters(x);
    }
    else
    if(model == "mlp")
    {
       ((NeuralSampler *)sampler)->setWeights(x);
    }
    else
    if(model == "nnc")
    {
        ((NNCSampler *)sampler)->setParameters(x);
    }

    for(int i=0;i<(int)x.size();i++)
        {
            double eps=pow(1e-18,1.0/3.0)*qMax(1.0,fabs(x[i]));
            x[i]+=eps;
            double v1=funmin(x);
            x[i]-=2.0 *eps;
            double v2=funmin(x);
            g[i]=(v1-v2)/(2.0 * eps);
            x[i]+=eps;
        }
}

FitProblem::~FitProblem()
{

    if(dataProblem!=NULL)
    {
        delete dataProblemWrapper;
        delete dataProblem;
    }
    if(sampler!=NULL)
        delete sampler;
}

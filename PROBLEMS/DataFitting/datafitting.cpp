#include "datafitting.h"
# include <math.h>
# include <math.h>
# include <math.h>
# include <interval.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <mlpmodel.h>
# include <rbfmodel.h>
# include <gdfmodel.h>
# include <nncmodel.h>
# include <rulermodel.h>
# include <polymodel.h>

using namespace std;

#pragma gcc optimize("Ofast")
extern "C"
{
QString model = "mlp";
int     weights = 10;
DataModel *demodel = NULL;
QString trainName  = "";
QString testName   = "";
DataSet *trainSet = NULL;
DataSet *testSet  = NULL;
void    init(QJsonObject data)
{
    if(data.contains("model"))
        model = data["model"].toString();
    if(data.contains("weights"))
        weights = data["weights"].toString().toInt();
    if(data.contains("trainName"))
        trainName = data["trainName"].toString();
    if(data.contains("testName"))
        testName = data["testName"].toString();
    if(trainName=="" || testName=="")
        exit(1);
    trainSet  = new DataSet(trainName.toStdString().c_str());
    testSet   = new DataSet(testName.toStdString().c_str());

    if (model == "mlp")
        demodel = new MlpModel(weights,trainSet,testSet);
    else
    if(model == "rbf")
        demodel = new RbfModel(weights,trainSet,testSet);
    else
    if(model == "nnc")
        demodel  = new NncModel(trainSet,testSet);
    else
    if(model == "gdf")
        demodel = new GdfModel(trainSet,testSet);
    else
    if(model == "ruler")
        demodel = new RulerModel(trainSet,testSet);
    else
    if(model == "poly")
        demodel = new PolyModel(weights,trainSet,testSet);
}

int	getdimension()
{
    if(model == "mlp")
        return  (trainSet->getdimension()+2)*weights;
    else
    if(model == "rbf")
        return ((RbfModel *)demodel)->getVariablesSize();
    else
    if(model == "nnc")
        return 10 * weights;
    else
    if(model == "gdf")
        return 10 * weights;
    else
    if(model == "ruler")
        return 10 * weights;
    else
    if(model == "poly")
        return trainSet->getdimension() * weights;
    return 1;
}
void    getmargins(vector<Interval> &x)
{
    if(model=="nnc" || model == "gdf" || model == "ruler")
    {
        for(int i=0;i<(int)x.size();i++)
            x[i]=Interval(0.0,255.0);
        return;
    }

        for(int i=0;i<(int)x.size();i++)
                x[i]=Interval(-100,100);
}

double funmin(vector<double> &a)
{
    double sum = 0.0;
    if(model == "mlp")
    {
        ((MlpModel *)demodel)->setWeights(a);
    }
    else
    if(model == "rbf")
    {
        ((RbfModel *)demodel)->setVariables(a);
    }
    else
    if(model == "nnc")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
        {
            ia[i]=(int)a[i];
            if(ia[i]<0) return 1e+100;
        }
        ((NncModel *)demodel)->setChromosome(ia);
    }
    else
    if(model=="gdf")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
        {
            ia[i]=(int)a[i];
            if(ia[i]<0 ) return 1e+100;
        }
        ((GdfModel *)demodel)->setChromosome(ia);
    }
    else
    if(model == "ruler")
    {
        vector<int> ia;
        ia.resize(a.size());
        for(int i=0;i<(int)a.size();i++)
        {
            ia[i]=(int)a[i];
            if(ia[i]<0) return 1e+100;
        }
        ((RulerModel *)demodel)->setChromosome(ia);
    }
    else
    if(model == "poly")
    {
        ((PolyModel *) demodel)->setWeights(a);
    }
    sum = demodel->getTrainError();
    return sum;
}
QJsonObject    done(vector<double> &x)
{
    double dv = funmin(x);
    double testError = 0.0;
    double classError = 0.0;
    QJsonObject jxx;
    testError = demodel->getTestError();
    classError = demodel->getClassError();
    if(model == "gdf")
    {

                QString st1=QString::fromStdString(

                    ((GdfModel *)demodel)->getLastExpr());

                jxx["string"]=st1;
    }
    else
    if(model == "ruler")
    {
        vector<int> ia;
        ia.resize(x.size());
        for(int i=0;i<(int)x.size();i++)
            ia[i]=(int)x[i];
        QString st1=QString::fromStdString(

            ((RulerModel *)demodel)->printF(ia));

        jxx["string"]=st1;
    }
    jxx["testError"]=testError;
    jxx["classError"]=classError;
    delete demodel;
    delete trainSet;
    delete testSet;
    return jxx;
}
static double dmax(double a,double b){return a>b?a:b;}
void	granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<getdimension();i++)
    {
        double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
        x[i]+=eps;
        double v1=funmin(x);
        x[i]-=2.0 *eps;
        double v2=funmin(x);
        g[i]=(v1-v2)/(2.0 * eps);
        x[i]+=eps;
    }
}
}

#include "rbftest.h"
# include <math.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <omp.h>
# include <dataset.h>
# include <kmeans.h>
# include <rbf.h>
# include <interval.h>
#include <adept.h>
#pragma GCC optimize("-Ofast")
using namespace std;
RbfTest::RbfTest()
{
}

extern "C"
{
QString trainName="xy.data";
QString testName="xy.data";
int nodes=10;
int dimension=0;
Dataset *train=NULL,*test=NULL;
vector<Data> trainx;
Data trainy;
vector<Data> testx;
Data testy;
Data dclass;
double initialLeft=-10000.0;
double initialRight= 10000.0;
int failCount=0;
int normalTrain=0;
QVector<Rbf*> myrbf;

//Rbf *myrbf[16]={NULL};

void loadTrain()
{
   train=new Dataset(trainName.toStdString().c_str());
}


void setParameter(QString name,QVariant value)
{

    if(name=="normalTrain")
    {
        normalTrain=value.toInt();
    }
}


void loadTest()
{
   test=new Dataset(testName.toStdString().c_str());
}


void    init(QJsonObject data)
{
    if(data.contains("trainName"))
        trainName=data["trainName"].toString();
    if(data.contains("testName"))
        testName=data["testName"].toString();
    if(data.contains("nodes"))
        nodes=data["nodes"].toInt();
    if(data.contains("initialLeft"))
        initialLeft=data["initialLeft"].toDouble();
    if(data.contains("initialRight"))
        initialRight=data["initialRight"].toDouble();
    loadTrain();
    if(testName!="xy.data") loadTest();
    myrbf.resize(32);
    for(unsigned i = 0; i <myrbf.size(); i++){
		
		myrbf[i]=new Rbf();
		myrbf[i]->setNumberOfWeights(nodes);
		if(train!=NULL) myrbf[i]->setTrainSet(train);
		if(test!=NULL)  myrbf[i]->setTestSet(test);
	}
}


int	getdimension()
{
    return nodes*train->getdimension()+nodes;
}


void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(initialLeft,initialRight);
    }
}

double	funmin(vector<double> &x)
{
    myrbf[omp_get_thread_num()]->setParameters(x);
    myrbf[omp_get_thread_num()]->train();
    double f=myrbf[omp_get_thread_num()]->getTrainError();
    return f;
}
double dmax(double a,double b){return a>b?a:b;}

//adept::Stack *s[16]={nullptr};

void    granal(vector<double> &x,vector<double> &g)
{
	
    for(int i=0;i<x.size();i++)
     {
         double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
         x[i]+=eps;
         double v1=funmin(x);
         x[i]-=2.0 *eps;
         double v2=funmin(x);
         g[i]=(v1-v2)/(2.0 * eps);
         x[i]+=eps;
     }
    return; 
    adept::Stack s1;

    myrbf[omp_get_thread_num()]->setParameters(x);
    myrbf[omp_get_thread_num()]->train();

     QVector<adept::adouble> aW(x.size());
     for(unsigned i = 0; i < x.size();i++){
		aW[i] = x[i];
	 }

     //s[omp_get_thread_num()]->new_recording();
     s1.new_recording();
     adept::adouble error = myrbf[omp_get_thread_num()]->getTrainError(aW);
     error.set_gradient(1.0);
     //s[omp_get_thread_num()]->compute_adjoint();
     s1.compute_adjoint();
     for(unsigned i = 0; i < x.size();i++){
		g[i] = aW[i].get_gradient();
	 }
     //s[omp_get_thread_num()]->clear();
     //delete s[omp_get_thread_num()];
     
}

QJsonObject    done(Data &x)
{
    QJsonObject result;
    result["nodes"]=nodes;
    result["testError"]=myrbf[omp_get_thread_num()]->getTestError();
    result["classError"]=myrbf[omp_get_thread_num()]->getClassError();
    result["string"]="RBF";
/*
    if(train!=NULL)
    delete train;
    if(test!=NULL)
    delete test;
    if(myrbf!=NULL)
    delete myrbf;
*/
    return result;
}


}

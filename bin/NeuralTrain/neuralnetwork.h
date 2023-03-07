#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
# include <Optimus/dataset.h>
# include <vector>
# include <stdio.h>
# include <iostream>
# include <QJsonObject>
# include <QFile>
# include <QTextStream>
# include <QDebug>
# include <adept.h>
# include <adept_arrays.h>

class NeuralNetwork
{
private:
    int normalTrain;
    int nodes;
    int dimension;
    vector<double> weight;
    vector<double> dclass;
    Dataset *trainSet;
public:
    NeuralNetwork(Dataset *tr,int N);
    double	dsig(double x);
    adept::adouble	adsig(adept::adouble x);
    double	dgetValue(Data &node,Data &x,int &fcount);
    adept::adouble	adgetValue(adept::aVector &node,Data &x,int &fcount);
    void	getDeriv(Data &node,Data &x,Data &G);
    void	getGradient(Data &node,Data &g);
    double	funmin(vector<double> &x);
    void    granal(vector<double> &x,vector<double> &g);
    double  nearestClass(double y);
    void    getOriginalGranal(Data &node,Data &g);
    adept::adouble afunmin(adept::aVector &Weights);
    double  getTrainError();
    double  getTestError(Dataset *tt);
    double  getClassTestError(Dataset *tt);
    void    disableNormalTrain();
    void    enableNormalTrain();
    ~NeuralNetwork();
};

#endif // NEURALNETWORK_H

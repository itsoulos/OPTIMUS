#ifndef RBFSAMPLER_H
#define RBFSAMPLER_H
# include "problem.h"

class RbfSampler
{
private:
    Problem *myProblem;
    int     nweights;
    Data    weight;
    int     dimension;
    vector<Data>    center;
    Data            variance;
    //for Problem Samples
    vector<Data>    xpoint;
    Data            ypoint;
    double      getDistance(Data &x,Data &y);
    double      gaussian(Data &x,Data &m,double v);
public:
    RbfSampler(Problem *p,int w);
    int     getNWeights() const;
    Data    getWeights();
    void    setWeights(Data &w);
    double  eval(Data &x);
    void    sampleFromProblem(int N);
    void    trainModel();
    void    sampleFromModel(int N,vector<Data> &xsample,Data &ysample);
    ~RbfSampler();
};

#endif // RBFSAMPLER_H

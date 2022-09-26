#ifndef RBFSAMPLER_H
#define RBFSAMPLER_H
# include "problem.h"

typedef vector<Data> Matrix;

class RbfSampler
{
private:
    Problem *myProblem;
    int     nweights;
    Data    weight;
    int     dimension;
    vector<Data>    center;
    Data            variance;
    vector<int> member;
    //for Problem Samples
    vector<Data>    xpoint;
    Data            ypoint;
    vector<int>     teamMembers;
    double      getDistance(Data &x,Data &y);
    double      gaussian(Data &x,Data &m,double v);
    void        initCenters();
    void        kmeans();
    int         nearestTeam(Data &x);
    void        updateCenters();
    Matrix      matrix_transpose(Matrix &x);
    Matrix      matrix_mult(Matrix &x,Matrix &y);
    Matrix      matrix_inverse(Matrix x,bool &ok);
    Matrix      matrix_pseudoinverse(Matrix &x,bool &ok);
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

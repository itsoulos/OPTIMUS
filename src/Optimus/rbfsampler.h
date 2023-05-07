#ifndef RBFSAMPLER_H
#define RBFSAMPLER_H
# include "problem.h"
# include "problemsampler.h"
# include "dataset.h"

/**
 * @brief The RbfSampler class samples points from the objective function using
 * an RBF network.
 */
class RbfSampler :public ProblemSampler
{
private:
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
    double      gaussianDerivative(Data &x,Data &m,double v,int pos);
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
    int     getParameterSize() const;
    void    setParameters(Data &x);
    Data    getParameters();
    void    setWeights(Data &w);
    double  eval(Data &x);
    Data    evalDeriv(Data &x);
    void    sampleFromProblem(int N,Matrix &xsample,Data &ysample);
    void    addSampleFromProblem(Data &x,double y);
    void    trainModel(QString method="");
    double  getTrainError();
    void    sampleFromModel(int &N,Matrix &xsample,Data &ysample);
    ~RbfSampler();
};

#endif // RBFSAMPLER_H
